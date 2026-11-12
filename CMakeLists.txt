cmake_minimum_required(VERSION 3.10)

project(FSM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Выбор файла с main:
#   cmake -S . -B build -DFSM_MAIN_SOURCE=main.cpp
#   cmake -S . -B build -DFSM_MAIN_SOURCE=d_statistics/main_d_statistics.cpp
set(FSM_MAIN_SOURCE "main.cpp" CACHE STRING
    "Source file containing the program entry point")
set_property(CACHE FSM_MAIN_SOURCE PROPERTY STRINGS
    "main.cpp"
    "d_statistics/main_d_statistics.cpp"
)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${FSM_MAIN_SOURCE}")
    message(FATAL_ERROR
        "FSM_MAIN_SOURCE does not exist: ${CMAKE_CURRENT_SOURCE_DIR}/${FSM_MAIN_SOURCE}")
endif()

# Библиотека проекта.
#
# FSM.cpp сейчас пустой, а FSM и FSMFactory являются header-only шаблонными
# компонентами, поэтому их .h-файлы не нужно компилировать отдельно.
#
# PermutationMain.cpp намеренно не включён: он содержит собственный main().
add_library(FSMlib
    include/Reverse.cpp
    include/Permutation/Permutation.cpp
    include/BinaryTree/BinaryTree.cpp
    include/Multi_Tree/MultiTree.cpp
)

target_include_directories(FSMlib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/BinaryTree
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Permutation
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Multi_Tree
    ${CMAKE_CURRENT_SOURCE_DIR}/include/FSMFactory
)

# Основное приложение. Для выбора d_statistics используйте FSM_MAIN_SOURCE
# из блока выше; одновременно собирается только один файл с main().
add_executable(FSM ${FSM_MAIN_SOURCE})
target_link_libraries(FSM PRIVATE FSMlib)

target_include_directories(FSM PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

set_target_properties(FSM PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "FSM main source: ${FSM_MAIN_SOURCE}")
