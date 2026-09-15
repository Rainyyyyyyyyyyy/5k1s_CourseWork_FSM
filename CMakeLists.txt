cmake_minimum_required(VERSION 3.10)
project(CodeGeneration)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Создание библиотеки из исходных файлов в src/
add_library(FSMlib
    #.../..../..../.cpp - исходники
    src/FSM.cpp

    
    #lib/Cpp/MethodUnit.cpp
    #src/PrintOperator.cpp
    #src/CppClassUnit.cpp
    #src/ClassUnit.cpp
)
target_include_directories(FSMlib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

# Создание исполняемого файла
add_executable(FSM main.cpp)
target_include_directories(FSM PRIVATE lib)
target_link_libraries(FSM FSMlib)

# add_executable(CodeGenerationTests test_main.cpp)
# target_include_directories(CodeGenerationTests PRIVATE lib)
# target_link_libraries(CodeGenerationTests FSMlib)

# Установка директории для выходного .exe файла
set_target_properties(FSM PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
