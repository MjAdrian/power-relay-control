add_library(crc8
    crc8.c
)

target_include_directories(crc8 PUBLIC
    $(CMAKE_CURRENT_SOURCE_DIR)
)

target_link_libraries(crc8
    pico_stdlib
)