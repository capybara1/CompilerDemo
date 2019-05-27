set(CMAKE_CXX_STANDARD 17)


# Use package antlr4-runtime
# See also https://github.com/antlr/antlr4/blob/master/runtime/Cpp/cmake/Antlr4Package.md#the-antlr4-runtime-package

find_package(
    antlr4-runtime
    REQUIRED
    PATHS /usr/local/lib/cmake/antlr4/
)
message(STATUS "ANTLR include dirs: ${ANTLR4_INCLUDE_DIRS}")

include_directories(${ANTLR4_INCLUDE_DIR})


# Use package antlr4-generator
# See also https://github.com/antlr/antlr4/blob/master/runtime/Cpp/cmake/Antlr4Package.md#the-antlr4-generator-package

find_package(
    antlr4-generator
    REQUIRED
    PATHS /usr/local/lib/cmake/antlr4/
)

set(ANTLR4_JAR_LOCATION /usr/local/lib/antlr-4.7.2-complete.jar)
antlr4_generate( 
    DemoGrammar
    ${CMAKE_CURRENT_SOURCE_DIR}/Demo.g4
    BOTH
    TRUE
    FALSE
    "compilerdemo"
)

include_directories(${ANTLR4_INCLUDE_DIR_DemoGrammar})


# USe package LLVM
# See also https://llvm.org/docs/CMake.html#embedding-llvm-in-your-project

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs core support bitwriter)


# Compile and link

add_executable(
    generator main.cpp DemoGenerator.cpp
    ${ANTLR4_SRC_FILES_DemoGrammar}
)

target_link_libraries(
    generator
    antlr4_static
    ${llvm_libs}
)
