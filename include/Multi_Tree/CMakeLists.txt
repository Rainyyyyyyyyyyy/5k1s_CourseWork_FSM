add_library(MultiTree STATIC
    MultiTree.cpp
)

target_include_directories(MultiTree PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
