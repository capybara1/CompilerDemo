#pragma once

#include <string>

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

#include <antlr4-runtime.h>

#include <DemoBaseListener.h>

namespace compilerdemo {

/**
 * This interface defines an abstract listener for a parse tree produced by DemoParser.
 */
class DemoGenerator : public DemoBaseListener {
private:

    llvm::LLVMContext context;
    std::unique_ptr<llvm::Module> module;
    llvm::IRBuilder<> builder;

    llvm::IRBuilder<>* mainFunctionBuilder;

    std::stack<llvm::Value*> valueStack;
    std::map<std::string, llvm::AllocaInst*> namedAllocations;

    llvm::Value* numberFormatString;
    llvm::Function* printFunction;
    llvm::Function* mainFunction;

public:

    DemoGenerator();

    virtual void enterProgram(DemoParser::ProgramContext* ctx) override;
    virtual void exitProgram(DemoParser::ProgramContext* ctx) override;

    virtual void enterAssignment(DemoParser::AssignmentContext* ctx) override;

    virtual void enterPrintStatement(DemoParser::PrintStatementContext* ctx) override;

    virtual void enterUnaryOperationExpression(DemoParser::UnaryOperationExpressionContext* ctx) override;

    virtual void enterVariableExpression(DemoParser::VariableExpressionContext* ctx) override;

    virtual void enterMulOperationExpression(DemoParser::MulOperationExpressionContext* ctx) override;

    virtual void enterDivOperationExpression(DemoParser::DivOperationExpressionContext* ctx) override;

    virtual void enterAddOperationExpression(DemoParser::AddOperationExpressionContext* ctx) override;

    virtual void enterSubOperationExpression(DemoParser::SubOperationExpressionContext* ctx) override;

    virtual void enterIntegerLiteral(DemoParser::IntegerLiteralContext* ctx) override;

    virtual void enterStringLiteral(DemoParser::StringLiteralContext* ctx) override;

private:

    void writeBitcodeToFile(std::string filePath);
    void emitGlobalConstants();
    void emitPrintfFunction();
    void emitStartMainFunction();
};

}  // namespace compilerdemo
