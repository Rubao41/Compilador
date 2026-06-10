cmake_minimum_required(VERSION 3.20)

project(compilador LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Avisa o compilador quais arquivos ele precisa juntar
add_executable(compilador
    main.cpp
    Lexer.cpp
    Parser.cpp
    Evaluator.cpp
)

target_compile_options(compilador PRIVATE
    -Wall
    -Wextra
    -pedantic
)