cmake_minimum_required(VERSION 3.16)
project(PascalProject)

set(CMAKE_CXX_STANDARD 14)

add_executable(PascalProject backend/Executor.cpp backend/Executor.h Simple.cpp Object.h frontend/Parser.cpp frontend/Parser.h frontend/Scanner.h frontend/Source.h frontend/Token.cpp frontend/Token.h intermediate/Node.h intermediate/ParseTreePrinter.cpp intermediate/ParseTreePrinter.h intermediate/Symtab.h intermediate/SymtabEntry.h)