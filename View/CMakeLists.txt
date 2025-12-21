add_library(view.o OBJECT
        graph/GraphView.cpp
        graph/GraphNode.cpp
        graph/PhysicsNode.cpp
        graph/GraphEdge.cpp
        menu/CircularMenu.cpp
        dialogs/NameDialog.cpp
        dialogs/TerminalDialog.cpp
        ViewModel.cpp
        dialogs/OutputDialog.cpp
        Shaders/shader.cpp
        Shaders/back.cpp
)

target_include_directories(view.o PRIVATE
        ${CTRL_DIR}/Terminal
        ${FYLESYS_DIR}
        ${FYLESYS_DIR}/Files
        ${COMMANDS_DIR}/interface
        ${COMMANDS_DIR}
        ${COMMANDS_DIR}/CommandManager
        ${MODEL_DIR}/AccessManager
        ${STL_DIR}
        ${VIEW_DIR}
        ${VIEW_DIR}/menu
        ${VIEW_DIR}/dialogs
        ${VIEW_DIR}/graph
        ${VIEW_DIR}/Shaders
)

target_link_libraries(view.o
        PRIVATE
        Qt5::Widgets
        Qt5::Core
        Qt5::Gui
)
