
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wno-write-strings -Wno-unused-variable -Wno-unused-function")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMY_DEBUG")

# Example main app
set(main_sources main.cpp Log.cpp)
add_executable(${PROJECT_NAME_LOWER}_main ${main_sources})
LIST(APPEND LIBRARIES dl) #NOTE: neccessary for all platforms?

# spdlog / log
if(CMAKE_SYSTEM_NAME STREQUAL "Android")
	LIST(APPEND LIBRARIES log)
else()
	LIST(APPEND LIBRARIES spdlog::spdlog)
endif()

# efsw
LIST(APPEND LIBRARIES efsw)
# FakeIt
#target_include_directories(${PROJECT_NAME_LOWER}_main ${PROJECT_SOURCE_DIR}/third_party/FakeIt)

target_link_libraries(${PROJECT_NAME_LOWER}_main ${PROJECT_NAME_LOWER} ${LIBRARIES})

