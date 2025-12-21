


add_library(filesystem.o OBJECT
        ${CMAKE_CURRENT_SOURCE_DIR}/filesys.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Files/files.cpp
)

target_include_directories(filesystem.o PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Files
        ${STL_DIR}
        ${MODEL_DIR}/PluginHandler
)
