#include <iostream>

#include <antlr4-runtime.h>

#include <DemoLexer.h>
#include <DemoParser.h>

#include "DemoGenerator.h"

using namespace compilerdemo;

int main(int argc, char** argv) {

    std::ifstream stream;
    stream.open(argv[0]);

    antlr4::ANTLRInputStream input(stream);
    DemoLexer lexer(&input);

    antlr4::CommonTokenStream tokens(&lexer);
    DemoParser parser(&tokens);

    DemoGenerator generator;

    antlr4::tree::ParseTreeWalker::DEFAULT.walk(&generator, parser.program());

    generator.writeBitcodeToFile(std::string("test"));

    return 0;
}
