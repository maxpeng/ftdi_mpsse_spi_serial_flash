# update the dependent sources.
set(CURRENT_EXE_NAME ftdi_mpsse_spi_serial_flash)

add_executable(${CURRENT_EXE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/main.c
    ${CMAKE_CURRENT_LIST_DIR}/dump_util.c
    ${CMAKE_CURRENT_LIST_DIR}/dump_util.h
    )

# add `src` directory to header search paths.
target_include_directories(${CURRENT_EXE_NAME}
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}"
    )

# load and run the CMake code from subdirectories for current target.
include(${CMAKE_CURRENT_LIST_DIR}/mpsse/CMakeLists.txt)
