add_library(pluginLoader.o OBJECT
        ${CMAKE_CURRENT_SOURCE_DIR}/pluginLoader.cpp
)

target_include_directories(pluginLoader.o PRIVATE
        ${MODEL_DIR}/Commands/interface
        ${MODEL_DIR}/Filesystem
        ${MODEL_DIR}/Filesystem/Files
        ${STL_DIR}
)
