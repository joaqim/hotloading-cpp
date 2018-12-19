cmake_minimum_required (VERSION 2.8)

cmake_policy(SET CMP0048 NEW)

project(musl_test VERSION 0.0.1)
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${PROJECT_SOURCE_DIR}/build-debug)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/plugins)
include_directories(${PROJECT_SOURCE_DIR}/apps)
include_directories(${PROJECT_SOURCE_DIR}/third_party)

include(version)
include(project_options)
include(dependencies)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(apps)
add_subdirectory(plugin)

