# Compiler Demo

## Terminology

### Derivation

> A derivation of a string for a grammar is a sequence of grammar rule applications that transform the start symbol into the string.
A derivation proves that the string belongs to the grammar's language.
([Source](https://en.wikipedia.org/wiki/Context-free_grammar#Derivations_and_syntax_trees))

> A strategy is followed that deterministically determines the next nonterminal to rewrite:
> - in a leftmost derivation, it is always the leftmost nonterminal;
> - in a rightmost derivation, it is always the rightmost nonterminal.
([Source](https://en.wikipedia.org/wiki/Context-free_grammar#Derivations_and_syntax_trees))

### Concrete Syntax Tree (CST)

- Also known as _parse tree_
- Output of a parser
- Complete representation of the input in the leafs
- Names of Rules in the interior nodes
  - How it is matched

### Abstract Syntax Tree (AST)

- Also known as _syntax tree_
- Output of a parser
- Free of inessential nodes
  - e.g. grouping parentheses are implicit in the tree structure
- Incomplete representation of the input in the leafs
- Names of Rules in the interior nodes
  - How it is matched

## Architecture

Front end
- Preprocessing
- Lexical analysis (lexing)
- Syntax analysis (parsing)
  - leads to an CST or AST
- Semantic analysis
- Generation of intermediate representation

Middle end
- Analysis
- Target independent optimizations

Back end
- Target specific optimizations
- Code generation or interpretation
- Assembly of object files (in case the generated code belongs to an assembly language)
- Linking

## Strategies

### Using an Existing Toolchain

Often an existing toolchain for a language such as C/C++ is used as middle and back end.
In this case the front end would transpile the custom language into C/C++.

This approach might provide a simple solution in case the custom language is close to C/C++.
Otherwise the mapping might turn out to be complex (e.g. if memory management by garbage collection is required),
which is a downside of the approach.

This problem is a motivation for the [LLVM project](#llvm).

## Parsers

### Types

Top-down parser
> first looks at the highest level of the parse tree and works down the parse tree by using the rewriting rules of a formal grammar
Top-down parsing can be viewed as an attempt to find left-most derivations
([Source](https://en.wikipedia.org/wiki/Top-down_parsing))

Bottom-up parser
> recognizes the text's lowest-level small details first, before its mid-level structures, and leaving the highest-level overall structure to last.
([Source](https://en.wikipedia.org/wiki/Bottom-up_parsing))

Shift-reduce parser
- Kind of bottom-up parser

Recursive descent parser
- Kind of top-down parser
- Uses mutually-recursive functions rather than tables
- Lower complexity (in many cases, no parser generator is required)
- Faster

Recursive ascent parser
- Kind of bottom-up parser
- Uses mutually-recursive functions rather than tables
- Lower complexity (in many cases, no parser generator is required)
- Faster

Table based parser
- Lower memory footprint
- Higher complexity (usually parser generator is required)

Packrat parser

### Supported Grammar

Motivation
- Placing restrictions on a context-free language grammar keeps runtime complexity at bay

LL(k)
- Top-Down
- Processes input left to right
- Leftmost derivation
- k is a lookahead to amke a decision

LL
- Synonym for LL(1)

LL(*)
- LL(k) which is not restricted to a finite number k

LF(k)
- Also known as strong LL-parser
- Top-Down

LL(k)
- Top-Down
- Processes input left to right
- Leftmost derivation
- k is a lookahead

LR(k)
- Bottom-Up-Parser
- Processes input left to right
- Rightmost derivation
- k is a lookahead to amke a decision

LR
- Synonym for LR(1)

LALR(1)
- Look-Ahead LR parser

### Parser Generation

- Also known as _compiler compiler_
- Usually supports generation of a lexer and parser based on a given grammar
- Among the most popular generators are Flex+Bison (LALR) and ANTLR (LL(*))

- [Comparison of parser generators](https://en.wikipedia.org/wiki/Comparison_of_parser_generators)

### Lex/Flex

- Generates a lexer
- Supported target language is C++

License

|Tool|License|
|----|-------|
|Lex |various|
|Flex|BSD    |

### Yacc/Bison

- Generates a parser
- Supported target language is C++
- Table driven (smaller memory footprint)

License

|Tool |License|
|-----|-------|
|Yacc |various|
|Bison|GPL    |

### ANTLR

- Generates a lexer, parser, syntax analyzer, visitor (tree walker) and/or listener based on a given LL(*) grammer (`.g4` format)
- Recursive descent parser
- Requires Java Runtime
- Supported target languages: Java, C#, Python, JavaScript, Go, C++, Swift
- An IDE tool, called ANTLRworks, is available
- May be used along with [StringTemplates](https://www.stringtemplate.org/) as shown in [this](https://theantlrguy.atlassian.net/wiki/spaces/ST/pages/1409118/Language+Translation+Using+ANTLR+and+StringTemplate) article

References

- [Let's build a compiler - wir entwickeln praxisnah einen Compiler mit Java und ANTLR4](https://www.youtube.com/playlist?list=PLOfFbVTfT2vbJ9qiw_6fWwBAmJAYV4iUm)
- [Antlr4 - Visitor vs Listener Pattern](https://saumitra.me/blog/antlr4-visitor-vs-listener-pattern/)

#### `.g4` format

- Supports labels for rules (`rule # label`)
- Supports labels for rule elements (`label=token`)

Resources

- [Grammars](https://github.com/antlr/antlr4/blob/master/doc/grammars.md)

#### Visitors and Listeners

|Feature                                   |Visitors|Listeners|
|------------------------------------------|:------:|:-------:|
|Explicit call required to include children|Yes     |No       |
|Use a return value                        |Yes     |No       |
|Support for large parse trees             |No      |Yes      |

References

- [Antlr4 - Visitor vs Listener Pattern](https://saumitra.me/blog/antlr4-visitor-vs-listener-pattern/)

#### Installation

Follow instuctions on [antlr.org](https://www.antlr.org/)

#### Demos

##### Show Syntax-Tree

```bash
antlr4 -o src Demo.g4
javac -cp lib/antlr.jar src/*.java
grun src/Demo expr
1+2*3
```
Emmit EOF (Linux: Ctrl+D, win: Ctrl+Z)

#### Target C++

```bash
antlr4 -o src -Dlanguage=Cpp -package theNamespace Demo.g4
```

Resources

- [C++](https://github.com/antlr/antlr4/blob/master/doc/cpp-target.md)

## Optimization and Code Generation

### LLVM

- Initially started by Chris Latner
- Motivated by the way JIT compilers worked at the time, which performed optimization during run-time. This would therefore be repeatet each time the application was executed unless the result is cached. Thus the working hypothesis was to move as much of the optimization to compile-time.
- Since typically programs are optimized by manipulating an intermediate representation (IR), the term Low Level Virtual Machine (LLVM) was coined.
- Since then LLVM developed to an umbrella project, featuring A core project as well as other subprojects concerned with the development of the `clang` front-end and various tools

Resources

- [Code generation to LLVM IR](https://llvm.org/docs/tutorial/LangImpl03.html)

#### Features

- Optimizer
- JIT
- GC

##### Intermediate Language

The IL can be provided in 2 representations:
- human readable ascii representation
- binary bitcode representation

Bidirectional transformation between representations is possible using the following commands:
- `llvm-as` (transforms IL in ascii representation to bitcode representation)
- `llvm-dis` (transforms IL in bitcode representation to ascii representation)

Language aspects:
- `;` introduces a line comment
- identifiers are prefixed according to their nature:
  - `@` for global identifiers ([functions](http://llvm.org/docs/LangRef.html#functions), [global variables](http://llvm.org/docs/LangRef.html#global-variables))
  - `%` for local identifiers (register names, types)
  - `$` for [comdats](http://llvm.org/docs/LangRef.html#comdats)
    - See also
      - [COMDAT sections](https://www.airs.com/blog/archives/52)
      - [Why is Identical COMDAT Folding called Identical COMDAT Folding?](https://devblogs.microsoft.com/oldnewthing/20161024-00/?p=94575)
      - See also [Wikipedia on the Object Module Format (OMF)](https://en.wikipedia.org/wiki/Relocatable_Object_Module_Format)
  - `!` for [metadata](http://llvm.org/docs/LangRef.html#metadata)
  - `#` for [attribute groups](http://llvm.org/docs/LangRef.html#attribute-groups)
  - `^` for a entry of a [module summary](http://llvm.org/docs/LangRef.html#thinlto-summary)
- Pointer types are speficied by `<type> *`
- Array types are speficied by `[ <size> x <type> ]`
- Vector types are speficied by `< <size> x <type> >`
- Structure types are specified by either `{ <type list> }` (regular) or `<{ <type list> }>` (packed)

In order to compile for a specific target additional information must be provided:
- a [datalayout](http://llvm.org/docs/LangRef.html#data-layout) which specifies
  - Byte order (endianness)
  - Type specific allocation sizes
  - Type specific memory alignments
  - Style of [name mangling](https://en.wikipedia.org/wiki/Name_mangling)
  - …
- a [target triple](http://llvm.org/docs/LangRef.html#target-triple) which specifies
  - Architecture
  - Vendor
  - Operating system
  - optional: Environment
- The [source filename](http://llvm.org/docs/LangRef.html#source-filename)

A module may also specify flags for upcoming stages in the toolchain such as e.g. [linker options](http://llvm.org/docs/LangRef.html#automatic-linker-flags-named-metadata)
and [magic lobal variables](http://llvm.org/docs/LangRef.html#intrinsic-global-variables).

#### Usage

- A front-end that outputs the IL is required
  - This may be achieved by utilization of the core libraries (C++) whilch provide classes such as e.g. an `IRBuilder`

References

- [LLVM Programmer’s Manual](http://llvm.org/docs/ProgrammersManual.html)
- [LLVM Language Reference Manual](http://llvm.org/docs/LangRef.html)

Commands (excerpt):
- `llvm-link` (linker for IL in bitcode representation)
- `lli` (executes IL in bitcode representation using JIT compilation)
- `lldb` (debugger)
- `opt` (optimizer)
- `llc` (compiles IL in any representation to assembly code)
    
See also [LLVM Command Guide](http://llvm.org/docs/CommandGuide/index.html) for a full listing

#### Installation

Build from source:
Requirements: Make, CMake, Compiler
Repository: https://github.com/llvm/llvm-project

Pre-build:
http://releases.llvm.org/download.html

#### Anatomy of the Optimizer

Based on multiple passes of different types:
- Module Pass
- Call Graph Pass
- Function Pass
- Basic Block Pass
- Immutable Pass, Region Pass, MachineFunction Pass

Within each of the mentioned passes:
- Analysis Pass
- Transform Pass

#### Demos

##### Get IL from C++ source using clang++

```bash
clang++ -S -emit-llvm source.cpp
```

Observations
- IL uses [SSA](https://en.wikipedia.org/wiki/Static_single_assignment_form)

##### Running a build-in pass

```bash
opt -dot-cfg-only input.ll > /dev/null
xdot cfg.main.dot
```

##### Running custom  pass

```cpp
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
    struct DemoPass : public FunctionPass {
        static char ID;
        DemoPass() : FunctionPass(ID) {}
        bool runOnFunction(Function &F) override {
            errs().write_escaped(F.getName()) << '\n';
            return false;
        }
    }
}
char DemoPass::ID = 0;
static RegisterPass<Hello> X("name_of_the_pass",
                             "Description of the pass",
                             false,
                             false);
```

```bash
opt -load path/to/DemoPass.so -name_of_the_pass < input.bc
```

### Resources

- [LLVM Compiler Infrastructure](https://llvm.org/docs/)
- [LLVM Tutorial: Table of Contents](http://llvm.org/docs/tutorial/)
- [LLVM](http://www.aosabook.org/en/llvm.html)
- [LLVM for Grad Students](https://www.cs.cornell.edu/~asampson/blog/llvm.html)
- [Introduction to LLVM Building simple program analysis tools and instrumentation](https://www.youtube.com/watch?v=VKIv_Bkp4pk)
- [YOW! 2016 Erik Corry - Building Your Own Compiler The Slightly Easier Way With LLVM](https://www.youtube.com/watch?v=2PVRX1ug_fk)
- [My First LLVM Compiler](http://www.wilfred.me.uk/blog/2015/02/21/my-first-llvm-compiler/)

## Demo Compiler

Build demo

1. Install ANTLR into `demo` directory
2. Install LLVM from source
3. Build demo using the following commands

```bash
cd demo
cmake .
cmake --build .
```