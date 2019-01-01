
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wno-write-strings -Wno-unused-variable -Wno-unused-function")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMY_DEBUG")

# Example main app
set(main_sources main.cpp )
add_executable(${PROJECT_NAME_LOWER}_main ${main_sources})
target_link_libraries(${PROJECT_NAME_LOWER}_main ${PROJECT_NAME_LOWER}
       	${LIBRARIES}
	Magnum::Application
	Magnum::GL
	Magnum::Magnum
	)

