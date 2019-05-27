#include <iostream>

#include <antlr4-runtime.h>

#include <DemoLexer.h>
#include <DemoParser.h>

#include "DemoGenerator.h"

using namespace compilerdemo;

int main(int argc, char** argv) {

    if (argc != 2)
    {
        std::cerr << "ERROR. Enter a file name";
        return 1;
    }

    std::ifstream stream;
    stream.open(argv[1]);

    antlr4::ANTLRInputStream input(stream);
    DemoLexer lexer(&input);

    antlr4::CommonTokenStream tokens(&lexer);
    DemoParser parser(&tokens);

    DemoGenerator generator;

    antlr4::tree::ParseTreeWalker::DEFAULT.walk(&generator, parser.program());

    generator.writeBitcodeToFile(std::string("test"));

    return 0;
}
