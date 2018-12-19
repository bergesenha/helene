cmake_minimum_required(VERSION 2.8)


project(helene CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

option(HELENE_BUILD_TESTS "Build unit tests" OFF)

add_subdirectory(external/metamusil)

add_library(helene INTERFACE)
target_compile_features(helene INTERFACE cxx_std_17)
target_include_directories(helene
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/
    )
target_link_libraries(helene INTERFACE metamusil)

if(HELENE_BUILD_TESTS)
    add_subdirectory(external/Catch)


    add_executable(unit_tests tests/test_main.cpp)
    target_link_libraries(unit_tests helene metamusil)
    target_include_directories(unit_tests
        PRIVATE external/Catch/include/
        )
    target_sources(unit_tests
        PRIVATE tests/test_member_iterator.cpp
        PRIVATE tests/test_member_iterator2.cpp
        PRIVATE tests/test_array_view.cpp
        PRIVATE tests/test_soa.cpp
        PRIVATE tests/test_circular_iterator.cpp
        PRIVATE tests/test_sliding_window.cpp
        PRIVATE tests/test_sliding_window_map.cpp
        PRIVATE tests/test_stable_container.cpp
        PRIVATE tests/test_handle_map.cpp
        PRIVATE tests/test_small_vector.cpp
        PRIVATE tests/test_tree.cpp
        PRIVATE tests/test_iterators.cpp
        PRIVATE tests/test_static_json.cpp
        )
endif(HELENE_BUILD_TESTS)
