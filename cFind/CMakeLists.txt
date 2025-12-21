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


set(PLUGIN_SRC
        replace_plugin.cpp
        dataRace.cpp
)

add_library(replace_plugin SHARED ${PLUGIN_SRC})

set_target_properties(replace_plugin PROPERTIES
        OUTPUT_NAME "replace"
        POSITION_INDEPENDENT_CODE ON
)

target_include_directories(replace_plugin PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${COMMAND_DIR}/CommandManager
        ${COMMAND_DIR}/interface
        ${COMMAND_DIR}/impl/include
        ${MODEL_DIR}/Filesystem
        ${MODEL_DIR}/Filesystem/Files
        ${STL_DIR}
        ${COMMAND_DIR}/extraCommands
        ${MODEL_DIR}/PlaginHandler
)

find_package(Threads REQUIRED)
target_link_libraries(replace_plugin PRIVATE Threads::Threads)

add_library(filesystem_obj OBJECT
        ${MODEL_DIR}/Filesystem/filesys.cpp
        ${MODEL_DIR}/Filesystem/Files/files.cpp
)

target_include_directories(filesystem_obj PRIVATE
        ${MODEL_DIR}/Filesystem
        ${MODEL_DIR}/Filesystem/Files
        ${STL_DIR}
)

target_sources(replace_plugin PRIVATE
        $<TARGET_OBJECTS:filesystem_obj>
)

find_package(TBB REQUIRED)

target_link_libraries(TEST PRIVATE TBB::tbb)
target_link_libraries(replace_plugin PRIVATE TBB::tbb)
