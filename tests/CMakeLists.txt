include_directories(${PROJECT_SOURCE_DIR}/third_party/catch2 ${PROJECT_SOURCE_DIR}/src)


if(${PROJECT_NAME}_BUILD_TESTS)
    file(GLOB test_sources *.cpp)

    add_executable(test_runner
        ${test_sources}
    )
    target_link_libraries(test_runner ${PROJECT_NAME_LOWER})

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
    add_test(test_runner test_runner)
endif()

