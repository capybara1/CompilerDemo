# See also https://github.com/antlr/antlr4/tree/master/runtime/Cpp/cmake,
# https://llvm.org/docs/CMake.html#embedding-llvm-in-your-project

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 17)


add_definitions(-DANTLR4CPP_STATIC)
set(ANTLR4_WITH_STATIC_CRT OFF)
include(ExternalAntlr4Cpp)
message(STATUS "ANTLR include dirs: ${ANTLR4_INCLUDE_DIRS}")

include_directories(${ANTLR4_INCLUDE_DIRS})


set(ANTLR_EXECUTABLE /usr/local/lib/antlr-4.7.2-complete.jar)
find_package(ANTLR REQUIRED)

antlr_target(
    DemoGrammar Demo.g4 BOTH
    PACKAGE compilerdemo
    LISTENER)
message(STATUS "ANTLR generator output dir: ${ANTLR_DemoGrammar_OUTPUT_DIR}")

include_directories(${ANTLR_DemoGrammar_OUTPUT_DIR})


find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs core support bitwriter)


add_executable(
    Main main.cpp DemoGenerator.cpp
    ${ANTLR_DemoGrammar_CXX_OUTPUTS})

target_link_libraries(
    Main
    antlr4_static
    ${llvm_libs})
