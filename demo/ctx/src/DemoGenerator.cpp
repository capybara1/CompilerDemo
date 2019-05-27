#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Support/FileSystem.h"

#include "DemoGenerator.h"

using namespace compilerdemo;

DemoGenerator::DemoGenerator():
    context(), // See http://llvm.org/docs/ProgrammersManual.html#achieving-isolation-with-llvmcontext
    module(llvm::make_unique<llvm::Module>("demo", context)), // See http://llvm.org/docs/ProgrammersManual.html#the-module-class
    builder(context)
{}

void DemoGenerator::writeBitcodeToFile(std::string filePath)
{
    std::error_code errorCode;
    llvm::raw_fd_ostream outStream("module", errorCode, llvm::sys::fs::F_None);
    llvm::WriteBitcodeToFile(*(this->module), outStream);
    outStream.flush();
}

void DemoGenerator::enterProgram(DemoParser::ProgramContext* ctx)
{
    this->emitGlobalConstants();
    this->emitPrintFunction();
    this->emitStartMainFunction();
}

void DemoGenerator::emitGlobalConstants()
{
    this->numberFormatString = this->builder.CreateGlobalString("%.2f", "numberFormatString");
}

void DemoGenerator::emitPrintFunction()
{
    auto printFunctionReturnType = llvm::Type::getInt32Ty(this->context);
    std::vector<llvm::Type*> printFunctionArgumentTypeList = {
        llvm::Type::getInt8Ty(this->context)->getPointerTo()};
    auto printFunctionType = llvm::FunctionType::get(
        printFunctionReturnType,
        printFunctionArgumentTypeList,
        false);
    this->printFunction = llvm::Function::Create(
        printFunctionType,
        llvm::Function::ExternalLinkage,
        "print",
        *(this->module));
}

void DemoGenerator::emitStartMainFunction()
{
    auto mainFunctionReturnType = llvm::Type::getInt32Ty(this->context);
    std::vector<llvm::Type*> mainFunctionArgumentTypeList = {
        llvm::Type::getInt32Ty(this->context),
        llvm::Type::getInt8Ty(this->context)->getPointerTo()->getPointerTo()};
    auto mainFunctionType = llvm::FunctionType::get(
        mainFunctionReturnType,
        mainFunctionArgumentTypeList,
        false);
    this->mainFunction = llvm::Function::Create(
        mainFunctionType,
        llvm::Function::ExternalLinkage,
        "main",
        (this->module));

    this->mainFunction->setCallingConv(llvm::CallingConv::C);

    auto args = this->mainFunction->arg_begin();
    auto argc = args++;
    argc->setName("argc");
    auto argv = args++;
    argv->setName("argv");
    auto block = llvm::BasicBlock::Create(
        this->context,
        "entry",
        this->mainFunction);

    this->mainFunctionBuilder = new llvm::IRBuilder<>(block);
}

void DemoGenerator::exitProgram(DemoParser::ProgramContext* ctx)
{
    auto returnValue = llvm::ConstantInt::get(llvm::Type::getInt32Ty(this->context), 0);
    this->mainFunctionBuilder->CreateRet(returnValue);

    delete this->mainFunctionBuilder;
    this->mainFunctionBuilder = nullptr;

    llvm::verifyModule(*(this->module), &llvm::errs());
}

void DemoGenerator::enterAssignment(DemoParser::AssignmentContext* ctx)
{
    auto variableName = ctx->IDENTIFIER()->getText();

    llvm::AllocaInst* allocation;
    auto it = this->namedAllocations.find(variableName);
    if (it != this->namedAllocations.end())
    {
        allocation = it->first;
    }
    else
    {
        allocation = this->mainFunctionBuilder->CreateAlloca(
            llvm::Type::getDoubleTy(this->context),
            nullptr,
            variableName);
        this->namedAllocations.emplace(variableName, allocation);
    }

    auto value = this->valueStack.top();
    this->valueStack.pop();
    this->mainFunctionBuilder->CreateStore(
        value,
        allocation);
}

void DemoGenerator::enterPrintStatement(DemoParser::PrintStatementContext* ctx)
{
    auto value = this->valueStack.top();
    this->valueStack.pop();
    this->mainFunctionBuilder->CreateCall(
        this->printFunction,
        std::vector<llvm::Value*>{this->numberFormatString, value});
}


void DemoGenerator::enterUnaryOperationExpression(DemoParser::UnaryOperationExpressionContext* ctx)
{
    auto value = this->valueStack.top();
    this->valueStack.pop();
    auto result = this->builder.CreateFNeg(value);
    this->valueStack.push(result);
}

void DemoGenerator::enterVariableExpression(DemoParser::VariableExpressionContext* ctx)
{
    auto variableName = ctx->IDENTIFIER()->getText();

    llvm::AllocaInst* allocation;
    try
    {
        allocation = this->namedAllocations.at(variableName);
    }
    catch (std::out_of_range& exception)
    {
        // ToDo: handle access to undeclared variable
        throw;
    }

    auto result = this->mainFunctionBuilder->CreateLoad(allocation);
    this->valueStack.push(result);
}

void DemoGenerator::enterMulOperationExpression(DemoParser::MulOperationExpressionContext* ctx)
{
    auto rhs = this->valueStack.top();
    this->valueStack.pop();
    auto lhs = this->valueStack.top();
    this->valueStack.pop();
    auto result = this->mainFunctionBuilder->CreateFMul(lhs, rhs);
    this->valueStack.push(result);
}

void DemoGenerator::enterDivOperationExpression(DemoParser::DivOperationExpressionContext* ctx)
{
    auto rhs = this->valueStack.top();
    this->valueStack.pop();
    auto lhs = this->valueStack.top();
    this->valueStack.pop();
    auto result = this->mainFunctionBuilder->CreateFDiv(lhs, rhs);
    this->valueStack.push(result);
}

void DemoGenerator::enterAddOperationExpression(DemoParser::AddOperationExpressionContext* ctx)
{
    auto rhs = this->valueStack.top();
    this->valueStack.pop();
    auto lhs = this->valueStack.top();
    this->valueStack.pop();
    auto result = this->mainFunctionBuilder->CreateFAdd(lhs, rhs);
    this->valueStack.push(result);
}

void DemoGenerator::enterSubOperationExpression(DemoParser::SubOperationExpressionContext* ctx)
{
    auto rhs = this->valueStack.top();
    this->valueStack.pop();
    auto lhs = this->valueStack.top();
    this->valueStack.pop();
    auto result = this->mainFunctionBuilder->CreateFSub(lhs, rhs);
    this->valueStack.push(result);
}

void DemoGenerator::enterIntegerLiteral(DemoParser::IntegerLiteralContext* ctx)
{
    auto literalString = ctx->NUMBER()->getText();
    auto value = std::stod(literalString);
    auto result = llvm::ConstantFP::get(this->context, llvm::APFloat(value))

    this->valueStack.push(result);
}

void DemoGenerator::enterStringLiteral(DemoParser::StringLiteralContext* ctx)
{
    auto stringLength = ctx->CHAR().size();
    char buffer[stringLength + 1];
    buffer[stringLength] = '\0';
    for (std::vector<antlr4::tree::TerminalNode *> ::size_type i = 0; i < stringLength; i++) {
        buffer[i] = ctx->CHAR(1)->getText()[0];
    }
    auto result = this->builder.CreateGlobalString(buffer);

    this->valueStack.push(result);
}
