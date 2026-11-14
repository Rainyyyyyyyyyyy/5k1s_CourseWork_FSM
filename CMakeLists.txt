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

add_subdirectory(include/Permutation)
add_subdirectory(include/BinaryTree)
add_subdirectory(include/Multi_Tree)

# Библиотека проекта.
#
# FSM.cpp сейчас пустой, а FSM и FSMFactory являются header-only шаблонными
# компонентами, поэтому их .h-файлы не нужно компилировать отдельно.
#
# FSMlib объединяет header-only код FSM/Reverse и зависимости деревьев.
add_library(FSMlib INTERFACE)

target_include_directories(FSMlib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/FSMFactory
)
target_link_libraries(FSMlib INTERFACE
    BinaryTree
    MultiTree
)

# Основное приложение. Для выбора d_statistics используйте FSM_MAIN_SOURCE
# из блока выше; одновременно собирается только один файл с main().
add_executable(FSM ${FSM_MAIN_SOURCE})
target_link_libraries(FSM PRIVATE
    FSMlib
    Permutation
)

target_include_directories(FSM PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# FSMFactoryTests - тесты для FSMFactory, которые проверяют корректность
# работы фабрики FSM<M>
add_executable(FSMFactoryTests
    ${CMAKE_CURRENT_SOURCE_DIR}/include/FSMFactory/FSM_with_factory_tests.cpp
)
target_include_directories(FSMFactoryTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Проверка критерия CriteriaGu от 27.10.2026
add_executable(CheckCryteriaGu
    CheckCryteriaGu/check1.cpp
)
target_link_libraries(CheckCryteriaGu PRIVATE
    Permutation
)
target_include_directories(CheckCryteriaGu PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(FSM PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "FSM main source: ${FSM_MAIN_SOURCE}")
