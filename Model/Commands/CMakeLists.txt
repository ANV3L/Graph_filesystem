


add_library(commands.o OBJECT
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/src/DirectoryCommands.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/src/FileCommands.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/src/RegularFileCommands.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/src/commands.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/CommandManager/commandManager.cpp
        ${MODEL_DIR}/PluginHandler/pluginLoader.cpp
)

target_include_directories(commands.o PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/CommandManager
        ${CMAKE_CURRENT_SOURCE_DIR}/interface
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/include
        ${MODEL_DIR}/Filesystem
        ${MODEL_DIR}/Filesystem/Files
        ${STL_DIR}
        ${CMAKE_SOURCE_DIR}/extraCommands
        ${MODEL_DIR}/PlaginHandler
)
