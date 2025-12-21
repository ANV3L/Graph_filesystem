set(TESTS_SRC
        accessManager_test.cpp
        files_test.cpp
        filesys_test.cpp
        command_test.cpp
        commandManager_test.cpp
        directoryCommands_test.cpp
        fileCommands_test.cpp
        regularFileCommands_test.cpp
        pluginLoader_test.cpp
)

add_library(model_tests OBJECT ${TESTS_SRC})

target_include_directories(model_tests PRIVATE
        ${MODEL_DIR}/AccessManager
        ${MODEL_DIR}/Filesystem
        ${MODEL_DIR}/Filesystem/Files
        ${MODEL_DIR}/Commands/interface
        ${MODEL_DIR}/Commands/CommandManager
        ${MODEL_DIR}/Commands/impl/include
        ${MODEL_DIR}/PluginHandler
        ${STL_DIR}
)

target_sources(model_tests PRIVATE
        ${MODEL_DIR}/Commands/impl/src/DirectoryCommands.cpp
        ${MODEL_DIR}/Commands/impl/src/FileCommands.cpp
        ${MODEL_DIR}/Commands/impl/src/RegularFileCommands.cpp
        ${MODEL_DIR}/Commands/impl/src/commands.cpp
        ${MODEL_DIR}/Commands/CommandManager/commandManager.cpp
        ${MODEL_DIR}/PluginHandler/pluginLoader.cpp
        ${MODEL_DIR}/Filesystem/filesys.cpp
        ${MODEL_DIR}/Filesystem/Files/files.cpp
        ${MODEL_DIR}/AccessManager/accessManager.cpp
)

target_link_libraries(model_tests PRIVATE coverage)