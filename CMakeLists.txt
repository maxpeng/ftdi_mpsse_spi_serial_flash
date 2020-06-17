cmake_minimum_required(VERSION 3.10)

project(ftdi_mpsse_spi_serial_flash)

enable_testing()

set(CMAKE_CXX_STANDARD 14)

# use -std=c++14 instead of -std=gnu++14
# refer https://crascit.com/2015/03/28/enabling-cxx11-in-cmake/
set(CMAKE_CXX_EXTENSIONS OFF)

# make build verbose by default.
set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON")


option(STATIC_LINK "use static link" OFF)
option(NOSPDLOG "disable spdlog" OFF)

# load and run the CMake code from subdirectories for current target.
include(src/CMakeLists.txt)

#add_subdirectory(tests)

