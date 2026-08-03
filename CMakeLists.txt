# For more information about the build system, see:
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
cmake_minimum_required(VERSION 3.16)

# Suppress CMake developer warnings emitted by the ESP-IDF build scripts.
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
set(BUILD_WARN_EFFECTIVE_POLICY OFF CACHE BOOL "" FORCE)
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS ON CACHE BOOL "" FORCE)

set(EXTRA_COMPONENT_DIRS
    "${CMAKE_CURRENT_LIST_DIR}/components/BSP"
    "${CMAKE_CURRENT_LIST_DIR}/components/RESOURCE"
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(02-demo)
