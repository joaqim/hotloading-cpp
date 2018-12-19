# Find dependencies

# Threads (need to do this explicitly on Linux?)
if (UNIX AND NOT APPLE)
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    find_package(Threads REQUIRED)
    if(CMAKE_USE_PTHREADS_INIT)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    endif()
endif()

# dlopen, dlsym etc . 
if (UNIX AND NOT APPLE) 
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

# spdlog
add_subdirectory(
	${CMAKE_SOURCE_DIR}/repos/spdlog
	${CMAKE_BINARY_DIR}/spdlog)
include_directories(${CMAKE_SOURCE_DIR}/repos/spdlog/include)

if(CMAKE_SYSTEM_NAME STREQUAL "Android")
	find_library(log-lib log)
	#NOTE: Link it as to target with spdlog::spdlog
else()
	#NOTE: Link to target as spdlog::spdlog
endif()

file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/build-debug/logs")

# efsw 
add_subdirectory(
	${CMAKE_SOURCE_DIR}/third_party/efsw
       	${CMAKE_BINARY_DIR}/efsw)
include_directories(${CMAKE_SOURCE_DIR}/third_party/efsw/include)

# Fake It
include_directories(${CMAKE_SOURCE_DIR}/third_party/FakeIt)
include_directories(${CMAKE_SOURCE_DIR}/third_party/catch2)
