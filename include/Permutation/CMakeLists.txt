add_library(Permutation STATIC
    Permutation.cpp
)

target_include_directories(Permutation PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
