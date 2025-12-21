cmake_minimum_required(VERSION 3.28)
project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_TLS_VERIFY OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS
        Core
        Widgets
        Gui
)

set(STL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/STL)

set(MODEL_DIR ${CMAKE_SOURCE_DIR}/Model)
set(FYLESYS_DIR ${MODEL_DIR}/Filesystem)
set(COMMANDS_DIR ${MODEL_DIR}/Commands)
set(CTRL_DIR ${CMAKE_SOURCE_DIR}/Controller)
set(VIEW_DIR ${CMAKE_SOURCE_DIR}/View)

include (FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

set (gtest_force_shared_crt ON CACHE BOOL "" FORCE) #Совместимость с виндой

FetchContent_MakeAvailable(googletest)

enable_testing()

include(GoogleTest)

add_subdirectory(STL)

add_subdirectory(Model)
add_subdirectory(View)
add_subdirectory(Controller)
add_subdirectory(cFind)

add_library(Filesystem STATIC)

target_link_libraries(Filesystem PUBLIC
        terminal.o
        filesystem.o
        commands.o
        view.o
)



# ---------------- main ------------

add_executable(main main.cpp)

target_link_libraries(main PRIVATE
        Filesystem
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
)
target_include_directories(main PRIVATE
        ${CTRL_DIR}/Terminal
        ${MODEL_DIR}/Filesystem
        ${MODEL_DIR}/Filesystem/Files
        ${STL_DIR}
        ${MODEL_DIR}/Commands/CommandManager
        ${MODEL_DIR}/Commands/interface
        ${MODEL_DIR}/Commands/impl/include
        ${MODEL_DIR}/AccessManager
        ${VIEW_DIR}
        ${MODEL_DIR}/PluginHandler
)

# -------------------- ALLTEST ----------------------

add_executable(ALLTEST)

target_sources(ALLTEST PRIVATE
        $<TARGET_OBJECTS:STL_TEST>
        $<TARGET_OBJECTS:model_tests>
)

target_link_libraries(ALLTEST PRIVATE
        STL_TEST
        model_tests
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ALLTEST)

add_library(coverage INTERFACE)

target_compile_options(coverage INTERFACE
        -O0 -g
        --coverage
        -fprofile-abs-path
)
target_link_options(coverage INTERFACE
        --coverage
)

target_link_libraries(ALLTEST PRIVATE coverage)
