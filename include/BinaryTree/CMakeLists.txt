add_library(BinaryTree STATIC
    BinaryTree.cpp
)

target_include_directories(BinaryTree PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)
