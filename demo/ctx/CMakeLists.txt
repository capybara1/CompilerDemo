# See also https://github.com/antlr/antlr4/tree/master/runtime/Cpp/cmake

cmake_minimum_required(
    VERSION 3.13
    FATAL_ERROR)
    
project(
    CompilerDemo
    VERSION 1.0
    LANGUAGES CXX)

add_subdirectory(src)
