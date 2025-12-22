cmake_minimum_required(VERSION 3.28)
project(PLUGIN)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PLUGIN_SRC
    plugin.cpp
)

add_library(plugin SHARED ${PLUGIN_SRC})

set_target_properties(plugin PROPERTIES
        OUTPUT_NAME "OK"
        POSITION_INDEPENDENT_CODE ON
)
