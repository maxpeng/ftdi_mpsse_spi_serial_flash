cmake_minimum_required(VERSION 3.10)

project(ftdi_mpsse_spi_serial_flash)

set(CMAKE_C_STANDARD 11)

# make build verbose by default.
set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON")

# load and run the CMake code from subdirectories for current target.
include(src/CMakeLists.txt)

