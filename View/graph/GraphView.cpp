#include "GraphView.hpp"

#include "GraphNode.hpp"
#include "PhysicsNode.hpp"
#include "CircularMenu.hpp"
#include "NameDialog.hpp"
#include "TerminalDialog.hpp"
#include "../Controller/Terminal/terminal.hpp"

#include <sstream>
#include <QGraphicsScene>
#include <QTimer>
#include <QWheelEvent>
#include <QColorDialog>
#include <QKeyEvent>
#include <QLinearGradient>
#include <QDateTime>
#include <QDebug>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <random>
#include <cmath>
#include <algorithm>
#include "OutputDialog.hpp"
#include "OutputDialog.hpp"
#include "GraphEdge.hpp"
#include "back.hpp"
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>

GraphView* g_graphView = nullptr;

void GraphView::wheelEvent(QWheelEvent* event) {
    if (event) event->accept();
}

GraphView::GraphView(Terminal *term, QWidget *parent)
    : terminal_(term), QGraphicsView(parent) {
    g_graphView = this;
    scene = new QGraphicsScene(this);

    QLinearGradient bgGrad(0, -600, 0, 600);
    bgGrad.setColorAt(0.0, QColor(5, 10, 30));
    bgGrad.setColorAt(0.5, QColor(3, 6, 20));
    bgGrad.setColorAt(1.0, QColor(0, 0, 10));
    scene = new StarfieldScene(this);
    scene->setSceneRect(-2000, -2000, 4000, 4000);
    setScene(scene);

    scene->setSceneRect(-2000, -2000, 4000, 4000);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);



    rootNode = new GraphNode(scene, QPointF(0, 0), "home", nodesColor_);
    currentNode = rootNode;
    allNodes.push_back(rootNode);
    currentNode->circle->setCurrent(true);

    physicsTimer = new QTimer(this);
    connect(physicsTimer, &QTimer::timeout, [this]() { updatePhysics(); });
    physicsTimer->start(16);

    menu = new CircularMenu(scene);

    commandItems = {
        "Открыть",
        "Удалить",
        "Переименовать",
        "Копировать",
        "Переместить"
    };

    menuMode = MenuMode::None;
    currentFileIndex = -1;

    deleteTimer = new QTimer(this);
    deleteTimer->setInterval(50);
    connect(deleteTimer, &QTimer::timeout, [this]() { updateDeleteAnimation(); });

    deleteActive = false;
    deleteNode = nullptr;
    deleteStartMs = 0;

    terminal = nullptr;

    centerOn(0, 0);
}

void GraphView::applyNodesBaseColor(const QColor& c) {
    nodesColor_ = c;

    for (auto* n : allNodes) {
        if (!n || !n->circle) continue;
        n->circle->setBaseColor(nodesColor_);
        n->circle->update();
    }

    for (auto* n : allNodes) {
        if (!n || !n->connectionLine || !n->parent || !n->parent->circle || !n->circle) continue;
        n->connectionLine->setEndpointColors(n->parent->circle->baseColor(),
                                             n->circle->baseColor());
        n->connectionLine->update();
    }
}



void GraphView::rebuildFromFileSystem(const FileSystem& fs) {
    if (rootNode) deleteSubtree(rootNode);
    allNodes.clear();

    rootNode = new GraphNode(scene, QPointF(0, 0), "home", nodesColor_);
    allNodes.push_back(rootNode);
    currentNode = rootNode;
    currentNode->circle->setCurrent(true);

    auto rootDir = fs.getRoot();
    if (!rootDir) return;

    std::function<void(std::shared_ptr<Directory>, GraphNode*, int)> build;
    build = [&](std::shared_ptr<Directory> dir, GraphNode* parentNode, int depth) {
        std::vector<std::shared_ptr<Directory>> subdirs;
        const auto files = dir->getFiles();
        for (auto it = files.begin(); it != files.end(); ++it) {
            if (auto d = std::dynamic_pointer_cast<Directory>(it->second))
                subdirs.push_back(d);
        }

        const qreal dx = 180.0;
        const qreal dy = 110.0;

        for (int i = 0; i < (int)subdirs.size(); ++i) {
            auto& childDir = subdirs[i];

            QPointF base = parentNode->circle->pos();
            QPointF pos  = base + QPointF(dx, (i - (subdirs.size() - 1) * 0.5) * dy);

            auto* childNode = new GraphNode(scene, pos, QString::fromStdString(childDir->getName()), nodesColor_);
            childNode->parent = parentNode;
            parentNode->children.push_back(childNode);
            allNodes.push_back(childNode);

            childNode->connectionLine = new GraphEdge(parentNode->circle, childNode->circle);
            scene->addItem(childNode->connectionLine);
            childNode->connectionLine->setZValue(-1);

            childNode->connectionLine->setEndpointColors(parentNode->circle->baseColor(),
                                            childNode->circle->baseColor());
            childNode->connectionLine->update();


            build(childDir, childNode, depth + 1);
        }
    };

    build(rootDir, rootNode, 0);

    updateHighlight();
    updateConnections();
    centerOn(0, 0);
}

static std::string buildNodePath(GraphNode* node) {
    if (node == nullptr) return {};

    std::vector<std::string> parts;
    GraphNode* cur = node;

    while (cur != nullptr) {
        if (cur->circle)
            parts.push_back(cur->circle->name.toStdString());
        cur = cur->parent;
    }

    std::reverse(parts.begin(), parts.end());

    std::string path;
    for (const auto& s : parts) {
        path += "/";
        path += s;
    }
    return path;
}


static void updateEdgesAround(GraphNode* node) {
    if (node == nullptr || node->circle == nullptr) return;

    if (node->connectionLine && node->parent && node->parent->circle) {
        node->connectionLine->setEndpointColors(
            node->parent->circle->baseColor(),
            node->circle->baseColor()
        );
        node->connectionLine->update();
    }

    for (GraphNode* ch : node->children) {
        if (ch == nullptr || ch->circle == nullptr || ch->connectionLine == nullptr) continue;

        ch->connectionLine->setEndpointColors(
            node->circle->baseColor(),
            ch->circle->baseColor()
        );
        ch->connectionLine->update();
    }
}


std::unordered_map<std::string, QColor> GraphView::exportViewColorConfig() const {
    std::unordered_map<std::string, QColor> cfg;

    for (GraphNode* node : allNodes) {
        if (node == nullptr || node->circle == nullptr) continue;

        const QColor c = node->circle->baseColor();
        if (c != nodesColor_) {
            cfg.emplace(buildNodePath(node), c);
        }
    }
    return cfg;
}


void GraphView::importViewColorConfig(const std::unordered_map<std::string, QColor>& cfg)
{
    if (cfg.empty()) return;

    for (GraphNode* node : allNodes) {
        if (node == nullptr || node->circle == nullptr) continue;

        const std::string path = buildNodePath(node);
        auto it = cfg.find(path);
        if (it == cfg.end()) continue;

        node->circle->setBaseColor(it->second);
        node->circle->update();

        if (node->connectionLine && node->parent && node->parent->circle) {
            updateEdgesAround(node);
        }
    }
}


void GraphView::addDirectoryNode(const QString& nodeName) {
    if (!currentNode) return;

    QPointF currentPos = currentNode->circle->pos();
    QPointF newPos = currentPos + QPointF(150, currentNode->children.size() * 80.0f);

    auto* newNode = new GraphNode(scene, newPos, nodeName, nodesColor_);
    newNode->parent = currentNode;
    currentNode->children.push_back(newNode);
    allNodes.push_back(newNode);

    newNode->connectionLine = new GraphEdge(currentNode->circle, newNode->circle);
    scene->addItem(newNode->connectionLine);
    newNode->connectionLine->setZValue(-1);
    newNode->connectionLine->setEndpointColors(currentNode->circle->baseColor(),
                                           newNode->circle->baseColor());
    newNode->connectionLine->update();



    updateHighlight();
}

void GraphView::applyZoom(double factor) {
    const double newZoom = zoomFactor * factor;
    if (newZoom < 0.15 || newZoom > 6.0) return;

    zoomFactor = newZoom;

    scale(factor, factor);
}


void GraphView::removeDirectoryByName(const QString& name) {
    if (!currentNode)

        return;

    GraphNode* target = nullptr;

    for (GraphNode* ch : currentNode->children) {
        if (!ch || !ch->circle)
            continue;
        if (ch->circle->name == name) {
            target = ch;
            break;
        }
    }
    if (!target)
        return;

    GraphNode* parent = target->parent;
    if (currentNode == target)
        currentNode = parent ? parent : rootNode;

    deleteSubtree(target);
    updateHighlight();
}


void GraphView::keyPressEvent(QKeyEvent *event) {
    int key = event->key();

    if (menu->visible) {
        handleMenuInput(key);
    } else {
        handleGraphInput(key);
    }

    if (key == Qt::Key_Left || key == Qt::Key_Right ||
        key == Qt::Key_Up || key == Qt::Key_Down ||
        key == Qt::Key_W || key == Qt::Key_A ||
        key == Qt::Key_S || key == Qt::Key_D ||
        key == Qt::Key_Q || key == Qt::Key_E ||
        key == Qt::Key_Z || key == Qt::Key_T ||
        key == Qt::Key_C || key == Qt::Key_V ||
        key == Qt::Key_F ||
        key == Qt::Key_Return || key == Qt::Key_Enter ||
        key == Qt::Key_Escape) {
        event->accept();
        return;
    }

    QGraphicsView::keyPressEvent(event);
}

void GraphView::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_X && deleteActive) {
        cancelDelete();
        event->accept();
        return;
    }
    QGraphicsView::keyReleaseEvent(event);
}


void GraphView::addStars(int count) {
    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<double> dx(-2000, 2000);
    std::uniform_real_distribution<double> dy(-2000, 2000);
    std::uniform_int_distribution<int> alpha(80, 180);
    std::uniform_int_distribution<int> size(1, 3);

    for (int i = 0; i < count; ++i) {
        int s = size(rng);
        auto *star = scene->addEllipse(
            -s / 2.0, -s / 2.0, s, s,
            QPen(Qt::NoPen),
            QBrush(QColor(200, 200, 255, alpha(rng)))
        );
        star->setPos(dx(rng), dy(rng));
        star->setZValue(-10);
    }
}

void GraphView::renameCurrentDirectory() {
    if (!currentNode || !currentNode->circle)
        return;

    QString oldName = currentNode->circle->name;
    if (oldName.isEmpty())
        return;

    QString newName = askName("Переименование директории");
    if (newName.isEmpty() || newName == oldName)
        return;

    if (terminal_) {
        try {
            std::string cmd = "rename . " + newName.toStdString();

            std::string res = terminal_->send(cmd);
            if (!res.empty()) {
                if (!res.empty()) {
                    qDebug() << "rename error:" << QString::fromStdString(res);
                    return;
                }
            }
        } catch (const std::exception& e) {
            qDebug() << "rename exception:" << e.what();
            return;
        }
    }

    currentNode->circle->name = newName;
    if (currentNode->circle->label) {
        currentNode->circle->label->setPlainText(newName);
        currentNode->circle->updateLabelPosition();
    }
    currentNode->circle->setToolTip(newName);
}



void GraphView::renameCurrentDirectoryViewOnly(const QString& newName) {
    if (!currentNode || !currentNode->circle)
        return;

    currentNode->circle->name = newName;
    if (currentNode->circle->label) {
        currentNode->circle->label->setPlainText(newName);
        currentNode->circle->updateLabelPosition();
    }
    currentNode->circle->setToolTip(newName);
}

void GraphView::renameChildDirectoryByName(const QString& oldName, const QString& newName) {
    if (!currentNode)
        return;

    for (GraphNode* ch : currentNode->children) {
        if (!ch || !ch->circle)
            continue;
        if (ch->circle->name == oldName) {
            ch->circle->name = newName;
            if (ch->circle->label) {
                ch->circle->label->setPlainText(newName);
                ch->circle->updateLabelPosition();
            }
            ch->circle->setToolTip(newName);
            return;
        }
    }
}



void GraphView::openTerminal() {
    if (!terminal) {
        terminal = new TerminalDialog(terminal_, this);
        g_terminal = terminal;
    }

    QRect parentGeom = this->frameGeometry();
    QPoint center = parentGeom.center();
    QRect termGeom = terminal->frameGeometry();

    terminal->move(center.x() - termGeom.width() / 2,
                   center.y() - termGeom.height() / 2);

    terminal->show();
    terminal->raise();
    terminal->activateWindow();
}


void GraphView::changeDirectoryViewOnly(const QString& target) {
    if (!currentNode)
        return;

    if (target == "..") {
        if (!currentNode->parent)
            return;

        currentNode->circle->setCurrent(false);
        currentNode = currentNode->parent;
        currentNode->circle->setCurrent(true);
        return;
    }

    for (GraphNode* ch : currentNode->children) {
        if (!ch || !ch->circle)
            continue;
        if (ch->circle->name == target) {
            currentNode->circle->setCurrent(false);
            currentNode = ch;
            currentNode->circle->setCurrent(true);
            return;
        }
    }
}



void GraphView::handleGraphInput(int key) {
    switch (key) {
        case Qt::Key_Plus:
        case Qt::Key_Equal:
            applyZoom(1.12);
            break;

        case Qt::Key_Minus:
        case Qt::Key_Underscore:
            applyZoom(1.0 / 1.12);
            break;

        case Qt::Key_Q:
            addDirectory();
            break;
        case Qt::Key_E:
            addFile();
            break;
        case Qt::Key_Z:
            renameCurrentDirectory();
            break;
        case Qt::Key_X:
            startDeleteCurrent();
            break;
        case Qt::Key_T:
            openTerminal();
            break;

        case Qt::Key_C: {
            QColor c = QColorDialog::getColor(
                currentNode ? currentNode->circle->baseColor() : nodesColor_,
                this,
                "Current node color"
            );
            if (c.isValid() && currentNode && currentNode->circle) {
                currentNode->circle->setBaseColor(c);
                currentNode->circle->update();
                if (currentNode->connectionLine && currentNode->parent) {
                    currentNode->connectionLine->setEndpointColors(currentNode->parent->circle->baseColor(),
                                                                   currentNode->circle->baseColor());
                    currentNode->connectionLine->update();
                }

                for (auto* ch : currentNode->children) {
                    if (!ch || !ch->connectionLine) continue;
                    ch->connectionLine->setEndpointColors(currentNode->circle->baseColor(),
                                                          ch->circle->baseColor());
                    ch->connectionLine->update();
                }

            }
            break;
        }



        case Qt::Key_V: {
            QColor c = QColorDialog::getColor(nodesColor_, this, "Nodes color");
            if (c.isValid()) applyNodesBaseColor(c);
            break;
        }


        case Qt::Key_H: {
            QString helpText =
                "Q  - create dir\n"
                "E  - create file\n"
                "Enter - choose file -> choose command\n"
                "Z  - rename directory\n"
                "X + space - delete directory (hold)\n"
                "WASD - move screen\n"
                "Arrows - move in graph\n"
                "T  - open terminal\n"
                "H  - show this help\n"
                "C - change color of current node\n"
                "V - change color of all nodes\n";

            OutputDialog dlg("Управление", helpText, this);
            dlg.exec();
            break;
        }


        case Qt::Key_W:
            moveCamera(0, -50);
            break;
        case Qt::Key_S:
            moveCamera(0, 50);
            break;
        case Qt::Key_A:
            moveCamera(-50, 0);
            break;
        case Qt::Key_D:
            moveCamera(50, 0);
            break;

        case Qt::Key_Left:
            moveToParent();
            break;
        case Qt::Key_Right:
            moveToFirstChild();
            break;
        case Qt::Key_Up:
            moveToPrevSibling();
            break;
        case Qt::Key_Down:
            moveToNextSibling();
            break;

        case Qt::Key_Return:
        case Qt::Key_Enter:
            openFileMenu();
            break;

        case Qt::Key_Escape:
            close();
            break;
    }
}

void GraphView::addFile() {

    QString name = askName("New file name");
    if (name.isEmpty())
        return;

    std::string cmd = "touch " + name.toStdString();
    terminal_->send(cmd);
}


void GraphView::handleMenuInput(int key) {
    switch (key) {
        case Qt::Key_Left:
            menu->prevPage();
            break;
        case Qt::Key_Right:
            menu->nextPage();
            break;
        case Qt::Key_Up:
            menu->prevItem();
            break;
        case Qt::Key_Down:
            menu->nextItem();
            break;

         case Qt::Key_Return:
        case Qt::Key_Enter:
            if (menuMode == MenuMode::Files) {
                QString file = menu->currentText();
                if (file.isEmpty())
                    break;

                selectedFileName_ = file.toStdString();

                commandItems.clear();
                if (terminal_) {
                    const auto& cmds = terminal_->getCmds();
                    commandItems.reserve(cmds.size());
                    for (const auto& s : cmds)
                        commandItems.push_back(QString::fromStdString(s));
                }

                menu->setItems(commandItems);
                if (currentNode && currentNode->circle)
                    menu->setThemeColor(currentNode->circle->baseColor());
                menu->show(currentNode->circle->pos());

                menuMode = MenuMode::Commands;

            } else if (menuMode == MenuMode::Commands) {
                if (selectedFileName_.empty()) {
                    menu->hide();
                    menuMode = MenuMode::None;
                    break;
                }

                QString cmdText = menu->currentText();
                if (cmdText.isEmpty()) {
                    menu->hide();
                    menuMode = MenuMode::None;
                    selectedFileName_.clear();
                    break;
                }

                QString fileName = QString::fromStdString(selectedFileName_);

                if (cmdText == "Открыть") {
                    if (terminal_) {
                        try {
                            std::string& buf =
                                terminal_->readFileToString(fileName.toStdString());

                            TextEditorDialog dlg("Редактор: " + fileName, buf, this);
                            dlg.exec();
                        } catch (const std::exception& e) {
                            qDebug() << "open editor error:" << e.what();
                        }
                    }
                } else {
                    QString args = askName("Аргументы команды (можно оставить пустым)");

                    QString line = cmdText + " " + fileName;
                    if (!args.isEmpty())
                        line += " " + args;

                    if (terminal_) {
                        try {
                            std::string res = terminal_->send(line.toStdString());
                            if (!res.empty()) {
                                OutputDialog dlg("Результат команды",
                                                 QString::fromStdString(res),
                                                 this);
                                dlg.exec();
                            }
                        } catch (const std::exception& e) {
                            OutputDialog dlg("Ошибка", e.what(), this);
                            dlg.exec();
                        }
                    }
                }

                menu->hide();
                menuMode = MenuMode::None;
                selectedFileName_.clear();
            }
            break;


case Qt::Key_Escape:
        menu->hide();
        menuMode = MenuMode::None;
        selectedFileName_.clear();
        break;

    default:
        break;
    }
}


void GraphView::moveCamera(qreal dx, qreal dy) {
    QPointF c = mapToScene(viewport()->rect().center());
    centerOn(c.x() + dx, c.y() + dy);
}


QString GraphView::askName(const QString &title) {
    NameDialog dlg(this);
    dlg.setWindowTitle(title);
    dlg.move(this->geometry().center() - dlg.rect().center());
    if (dlg.exec() != QDialog::Accepted)
        return {};
    QString name = dlg.text().trimmed();
    if (name.isEmpty())
        return {};
    return name;
}

void GraphView::addDirectory() {
    const QString nodeName = askName("Новая директория");
    if (nodeName.isEmpty()) return;

    const std::string cmd = "mkdir " + nodeName.toStdString();
    const std::string res = terminal_->send(cmd);
    if (!res.empty()) {
        terminalPrint(QString::fromStdString(res));
        std::cout << "'" << res << "'" << std::endl;
        if (res == "File already exist") return;
    }

    addDirectoryNode(nodeName);
}



void GraphView::openFileMenu() {
    if (!terminal_) return;

    std::string res;
    try {
        res = terminal_->send("lsRegular");
    } catch (const std::exception &) {
        return;
    }

    if (res.empty())
        return;

    std::istringstream iss(res);
    std::string token;
    std::vector<QString> fileItems;

    while (iss >> token) {
        fileItems.push_back(QString::fromStdString(token));
    }

    if (fileItems.empty())
        return;

    menu->setItems(fileItems);
    if (currentNode && currentNode->circle)
        menu->setThemeColor(currentNode->circle->baseColor());
    menu->show(currentNode->circle->pos());

    menuMode = MenuMode::Files;
    selectedFileName_.clear();
}


void GraphView::moveToParent() {
    if (!currentNode || !currentNode->parent)
        return;

    if (terminal_) {
        try {
            std::string res = terminal_->send("cd ..");
            if (!res.empty())
                qDebug() << "cd ..:" << QString::fromStdString(res);
        } catch (const std::exception& e) {
            qDebug() << "cd .. error:" << e.what();
            return;
        }
    }

        currentNode->circle->setCurrent(false);
        currentNode = currentNode->parent;
        currentNode->circle->setCurrent(true);

}

void GraphView::moveToFirstChild() {

    if (!currentNode || currentNode->children.empty())
        return;

    GraphNode* child = currentNode->children[0];
    QString dirName  = child->circle->name;

    if (terminal_) {
        try {
            std::string cmd = "cd " + dirName.toStdString();
            std::string res = terminal_->send(cmd);
            if (!res.empty())
                qDebug() << "cd child:" << QString::fromStdString(res);
        } catch (const std::exception& e) {
            qDebug() << "cd child error:" << e.what();
            return;
        }
    }

        currentNode->circle->setCurrent(false);
        currentNode = currentNode->children[0];
        currentNode->circle->setCurrent(true);

}

void GraphView::moveToPrevSibling() {
    if (!currentNode || !currentNode->parent)
        return;

    auto& siblings = currentNode->parent->children;
    for (size_t i = 0; i < siblings.size(); ++i) {
        if (siblings[i] == currentNode && i > 0) {
            GraphNode* target = siblings[i - 1];
            QString dirName   = target->circle->name;

            if (terminal_) {
                try {
                    std::string res = terminal_->send("cd ..");
                    if (!res.empty())
                        qDebug() << "cd ..:" << QString::fromStdString(res);

                    std::string cmd = "cd " + dirName.toStdString();
                    res = terminal_->send(cmd);
                    if (!res.empty())
                        qDebug() << "cd sibling:" << QString::fromStdString(res);
                } catch (const std::exception& e) {
                    qDebug() << "cd prev sibling error:" << e.what();
                    return;
                }
            }

            currentNode->circle->setCurrent(false);
            currentNode = target;
            currentNode->circle->setCurrent(true);
            return;
        }
    }
}

void GraphView::moveToNextSibling() {
    if (!currentNode || !currentNode->parent)
        return;

    auto& siblings = currentNode->parent->children;
    for (size_t i = 0; i < siblings.size(); ++i) {
        if (siblings[i] == currentNode && i + 1 < siblings.size()) {
            GraphNode* target = siblings[i + 1];
            QString dirName   = target->circle->name;

            if (terminal_) {
                try {
                    std::string res = terminal_->send("cd ..");
                    if (!res.empty())
                        qDebug() << "cd ..:" << QString::fromStdString(res);

                    std::string cmd = "cd " + dirName.toStdString();
                    res = terminal_->send(cmd);
                    if (!res.empty())
                        qDebug() << "cd sibling:" << QString::fromStdString(res);
                } catch (const std::exception& e) {
                    qDebug() << "cd next sibling error:" << e.what();
                    return;
                }
            }

            currentNode->circle->setCurrent(false);
            currentNode = target;
            currentNode->circle->setCurrent(true);
            return;
        }
    }
}


void GraphView::updateHighlight() {
    for (GraphNode *node: allNodes) node->circle->setCurrent(false);
    if (currentNode) currentNode->circle->setCurrent(true);
}

void GraphView::updateConnections() {
    for (GraphNode *node: allNodes) {
        if (node->connectionLine && node->parent && node->parent->circle) {
            QPointF p1 = node->parent->circle->pos();
            QPointF p2 = node->circle->pos();
            node->connectionLine->updatePath();
        }
    }
}


void GraphView::updatePhysics() {
    for (GraphNode *node: allNodes) node->circle->force = QPointF(0, 0);

    for (size_t i = 0; i < allNodes.size(); ++i) {
        for (size_t j = i + 1; j < allNodes.size(); ++j) {
            PhysicsNode *n1 = allNodes[i]->circle;
            PhysicsNode *n2 = allNodes[j]->circle;
            QPointF vec = n1->pos() - n2->pos();
            qreal dist = std::sqrt(vec.x() * vec.x() + vec.y() * vec.y());
            if (dist > 0 && dist < 250) {
                qreal repulsion = 5000.0 / (dist * dist);
                QPointF force = (vec / dist) * repulsion;
                n1->force += force;
                n2->force -= force;
            }
        }
    }

    for (GraphNode *node: allNodes) {
        if (node->parent && node->parent->circle) {
            PhysicsNode *parent = node->parent->circle;
            PhysicsNode *child = node->circle;
            QPointF vec = parent->pos() - child->pos();
            qreal dist = std::sqrt(vec.x() * vec.x() + vec.y() * vec.y());
            qreal idealDist = 150.0;
            if (dist > 0) {
                qreal springForce = (dist - idealDist) * 0.2;
                QPointF force = (vec / dist) * springForce;
                child->force += force;
                parent->force -= force;
            }
        }
    }

    for (GraphNode *node: allNodes) {
        PhysicsNode *phys = node->circle;
        if (scene->mouseGrabberItem() == phys) {
            phys->velocity *= 0.1;
            phys->updateLabelPosition();
            continue;
        }

        phys->velocity += phys->force * 0.02;
        phys->velocity *= 0.92;
        qreal speed = std::sqrt(phys->velocity.x() * phys->velocity.x() +
                                phys->velocity.y() * phys->velocity.y());
        if (speed > 8.0) {
            phys->velocity = (phys->velocity / speed) * 8.0;
        }

        phys->setPos(phys->pos() + phys->velocity);
        phys->updateLabelPosition();
    }

    updateConnections();

    if (menu->visible && currentNode && currentNode->circle) {
        menu->recenter(currentNode->circle->pos());
    }
}


void GraphView::startDeleteCurrent() {
    if (deleteActive) return;
    if (!currentNode) return;
    if (currentNode == rootNode) return;

    deleteActive = true;
    deleteNode = currentNode;
    deleteStartMs = QDateTime::currentMSecsSinceEpoch();
    deleteNode->circle->setDeleteProgress(0.0);
    deleteTimer->start();
}

void GraphView::updateDeleteAnimation() {
    if (!deleteActive || !deleteNode) {
        deleteTimer->stop();
        return;
    }

    qint64 now = QDateTime::currentMSecsSinceEpoch();
    double t = double(now - deleteStartMs) / 3000.0;
    if (t > 1.0)
        t = 1.0;

    deleteNode->circle->setDeleteProgress(t);

    if (t >= 1.0) {
        deleteTimer->stop();

        GraphNode* toDelete = deleteNode;
        deleteActive = false;
        deleteNode = nullptr;

        if (!toDelete)
            return;

        bool ok = deleteDirectoryInModel(toDelete);
        if (!ok) {
            toDelete->circle->setDeleteProgress(0.0);
            return;
        }

        GraphNode* parent = toDelete->parent;
        if (currentNode == toDelete)
            currentNode = parent ? parent : rootNode;

        deleteSubtree(toDelete);
        updateHighlight();
    }
}

bool GraphView::deleteDirectoryInModel(GraphNode* node) {
    if (!terminal || !node || !node->parent)
        return false;

    QString dirName = node->circle->name;
    if (dirName.isEmpty())
        return false;

    try {
        std::string res = terminal_->send("cd ..");
        if (!res.empty())
            qDebug() << "cd ..:" << QString::fromStdString(res);

        std::string cmd = "rm " + dirName.toStdString();
        res = terminal_->send(cmd);
        if (!res.empty())
            qDebug() << "rm:" << QString::fromStdString(res);

        return true;
    } catch (const std::exception& e) {
        qDebug() << "deleteDirectoryInModel error:" << e.what();
        return false;
    }
}


void GraphView::cancelDelete() {
    deleteActive = false;
    deleteTimer->stop();
    if (deleteNode) {
        deleteNode->circle->setDeleteProgress(0.0);
        deleteNode->circle->setCurrent(deleteNode == currentNode);
    }
    deleteNode = nullptr;
}

void GraphView::deleteSubtree(GraphNode *node) {
    if (!node) return;

    auto childrenCopy = node->children;
    for (GraphNode *ch: childrenCopy) {
        deleteSubtree(ch);
    }
    node->children.clear();

    if (node->parent) {
        auto &siblings = node->parent->children;
        siblings.erase(std::remove(siblings.begin(), siblings.end(), node),
                       siblings.end());
    }

    allNodes.erase(std::remove(allNodes.begin(), allNodes.end(), node),
                   allNodes.end());

    if (node->connectionLine) {
        scene->removeItem(node->connectionLine);
        delete node->connectionLine;
    }

    if (node->circle) {
        if (node->circle->label) {
            scene->removeItem(node->circle->label);
            delete node->circle->label;
        }
        scene->removeItem(node->circle);
        delete node->circle;
    }

    delete node;
}
