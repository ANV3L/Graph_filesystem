


add_library(terminal.o OBJECT
        ${MODEL_DIR}/AccessManager/accessManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/terminal.cpp
)

target_include_directories(terminal.o PRIVATE
        ${FYLESYS_DIR}
        ${FYLESYS_DIR}/Files
        ${COMMANDS_DIR}/interface
        ${COMMANDS_DIR}
        ${COMMANDS_DIR}/CommandManager
        ${MODEL_DIR}/AccessManager
        ${STL_DIR}
        ${VIEW_DIR}
)

target_link_libraries(terminal.o PRIVATE
        Qt5::Widgets
)
