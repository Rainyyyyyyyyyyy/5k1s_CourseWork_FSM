cmake_minimum_required(VERSION 3.10)
project(CodeGeneration)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Создание библиотеки из исходных файлов в src/
add_library(FSMlib
    #.../..../..../.cpp - исходники

    #../include/
    include/FSM.cpp
    include/Reverse.cpp

    #../include/Permutation/
    include/Permutation/Permutation.cpp


    #../include/BinaryTree/
    include/BinaryTree/BinaryTree.cpp
    include/Multi_Tree/MultiTree.cpp


    #include/Permutation.cpp
    
    #lib/Cpp/MethodUnit.cpp
    #src/PrintOperator.cpp
    #src/CppClassUnit.cpp
    #src/ClassUnit.cpp
)
target_include_directories(FSMlib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/BinaryTree
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Permutation
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Multi_Tree
    
    
    )

# Создание исполняемого файла
add_executable(FSM main.cpp) #include/PermutationMain.cpp)
target_include_directories(FSM PRIVATE lib)
target_link_libraries(FSM FSMlib)

# add_executable(CodeGenerationTests test_main.cpp)
# target_include_directories(CodeGenerationTests PRIVATE lib)
# target_link_libraries(CodeGenerationTests FSMlib)

# Установка директории для выходного .exe файла
set_target_properties(FSM PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# вывести в консоль компилятор
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
