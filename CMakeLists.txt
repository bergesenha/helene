cmake_minimum_required(VERSION 3.7)


project(helene CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(HELENE_BUILD_TESTS "Build unit tests" ON)

add_library(helene INTERFACE)
target_include_directories(helene INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/)

if(HELENE_BUILD_TESTS)
    add_subdirectory(external/Catch)


    set(HELENE_TEST_SRC
        tests/test_main.cpp
        tests/test_dag.cpp
        tests/test_iterators.cpp
        tests/test_algorithms.cpp
        tests/test_labeled_dag.cpp
        tests/test_member_iterator.cpp
        tests/test_member_iterator2.cpp
        )

    add_executable(unit_tests ${HELENE_TEST_SRC})
    target_link_libraries(unit_tests helene)
    target_include_directories(unit_tests
        PRIVATE external/Catch/include/
        )
    set_target_properties(unit_tests PROPERTIES
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        CXX_STANDARD 11
        )

endif(HELENE_BUILD_TESTS)
