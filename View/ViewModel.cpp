#include <QApplication>
#include "ViewModel.hpp"

void GraphFileSystem::run(int argc, char *argv[]) {
    QApplication app(argc, argv);

    GraphView view(&terminal_);
    view.setWindowTitle("Graph Navigation with Files (FX)");
    view.showFullScreen();


    app.exec();
}