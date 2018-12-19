file(GLOB src_SOURCES *.cpp)
set(lib${PROJECT_NAME_LOWER}_sources ${src_SOURCES})

add_library(${PROJECT_NAME_LOWER} ${lib${PROJECT_NAME_LOWER}_sources})

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


target_link_libraries(${PROJECT_NAME_LOWER} ${LIBRARIES})
