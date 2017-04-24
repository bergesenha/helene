cmake_minimum_required(VERSION 3.7)


project(helene CXX)


option(HELENE_BUILD_TESTS "Build unit tests" ON)


if(HELENE_BUILD_TESTS)
    add_subdirectory(external/Catch)


    set(HELENE_TEST_SRC
        tests/test_main.cpp
        )

    add_executable(unit_tests ${HELENE_TEST_SRC})
    target_include_directories(unit_tests
        PRIVATE external/Catch/include/
        PRIVATE include/
        )
    set_target_properties(unit_tests PROPERTIES
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        CXX_STANDARD 11
        )

endif(HELENE_BUILD_TESTS)
