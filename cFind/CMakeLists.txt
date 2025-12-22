set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project(cFind CXX)

set(TESTS_SRC
        tests.cpp
        dataRace.cpp
)

add_executable(TEST ${TESTS_SRC})

target_include_directories(TEST PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(TEST PRIVATE GTest::gtest_main)
gtest_discover_tests(TEST)

# ---------------- START OF PLUGIN ----------------------

set(PLUGIN_SRC
        replace_plugin.cpp
        dataRace.cpp
)

add_library(replace_plugin SHARED ${PLUGIN_SRC})

set_target_properties(replace_plugin PROPERTIES
        OUTPUT_NAME "replace"
        POSITION_INDEPENDENT_CODE ON
)

# ----------------- END OF PLUGIN -------------------------

find_package(Threads REQUIRED)
target_link_libraries(replace_plugin PRIVATE Threads::Threads)

find_package(TBB REQUIRED)

target_link_libraries(TEST PRIVATE TBB::tbb)
target_link_libraries(replace_plugin PRIVATE TBB::tbb)
