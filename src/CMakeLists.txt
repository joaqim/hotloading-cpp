file(GLOB src_SOURCES *.cpp)
set(lib${PROJECT_NAME_LOWER}_sources ${src_SOURCES})

add_library(${PROJECT_NAME_LOWER} ${lib${PROJECT_NAME_LOWER}_sources})
target_link_libraries(${PROJECT_NAME_LOWER} efsw)
