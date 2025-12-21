#pragma once

#include <string>
#include <vector>

#include <QGraphicsView>
#include <QColor>
#include <unordered_map>
#include <string>

class FileSystem;
class GraphNode;
class CircularMenu;
class TerminalDialog;
class Terminal;
class QGraphicsScene;
class QKeyEvent;
class QTimer;

/**
 * @brief Виджет визуализации дерева каталогов и навигации по нему
 */
class GraphView : public QGraphicsView {
    Q_OBJECT
public:
    /**
     * @brief Создаёт вид графа и связывает его с терминалом модели
     * @param term Указатель на терминал или nullptr
     * @param parent Родительский виджет или nullptr
     */
    explicit GraphView(Terminal* term, QWidget* parent = nullptr);

    /**
     * @brief Добавляет узел каталога в текущий каталог представления
     * @param nodeName Имя создаваемого каталога
     */
    void addDirectoryNode(const QString& nodeName);

    /**
     * @brief Удаляет узел по имени из представления
     * @param name Имя удаляемого узла
     */
    void removeDirectoryByName(const QString& name);

    /**
     * @brief Переименовывает текущий каталог только в представлении
     * @param newName Новое имя каталога
     */
    void renameCurrentDirectoryViewOnly(const QString& newName);

    /**
     * @brief Переименовывает дочерний каталог только в представлении
     * @param oldName Старое имя каталога
     * @param newName Новое имя каталога
     */
    void renameChildDirectoryByName(const QString& oldName, const QString& newName);

    /**
     * @brief Меняет текущий каталог только в представлении
     * @param target Имя целевого каталога
     */
    void changeDirectoryViewOnly(const QString& target);

    /**
     * @brief Перестраивает представление из модели файловой системы
     * @param fs Модель файловой системы
     */
    void rebuildFromFileSystem(const FileSystem& fs);

    std::unordered_map<std::string, QColor> exportViewColorConfig() const;

    void importViewColorConfig(const std::unordered_map<std::string, QColor>& cfg);

protected:
    /**
     * @brief Обрабатывает нажатия клавиш для управления графом и меню
     * @param event Событие клавиатуры
     */
    void keyPressEvent(QKeyEvent* event) override;

    /**
     * @brief Обрабатывает отпускание клавиш для управления графом и меню
     * @param event Событие клавиатуры
     */
    void keyReleaseEvent(QKeyEvent* event) override;

    void wheelEvent(QWheelEvent* event) override;



private:
    QColor glowColor_ = QColor(160, 205, 255, 60);
    QColor nodesColor_ = QColor(180, 210, 255);


    void applyNodesBaseColor(const QColor& c);

    enum class MenuMode {
        None,
        Files,
        Commands
    };

    void addStars(int count);
    bool deleteDirectoryInModel(GraphNode* node);
    void renameCurrentDirectory();
    void openTerminal();
    void handleGraphInput(int key);
    void handleMenuInput(int key);
    void moveCamera(qreal dx, qreal dy);
    QString askName(const QString& title);
    void addDirectory();
    void addFile();
    void openFileMenu();
    void moveToParent();
    void moveToFirstChild();
    void moveToPrevSibling();
    void moveToNextSibling();
    void updateHighlight();
    void updateConnections();
    void updatePhysics();
    void startDeleteCurrent();
    void updateDeleteAnimation();
    void cancelDelete();
    void applyZoom(double factor);
    void deleteSubtree(GraphNode* node);

    double zoomFactor = 1.0;

    QGraphicsScene* scene = nullptr;
    std::vector<GraphNode*> allNodes;
    GraphNode* rootNode = nullptr;
    GraphNode* currentNode = nullptr;
    QTimer* physicsTimer = nullptr;
    CircularMenu* menu = nullptr;

    MenuMode menuMode = MenuMode::None;
    int currentFileIndex = -1;
    std::vector<QString> commandItems;

    QTimer* deleteTimer = nullptr;
    bool deleteActive = false;
    qint64 deleteStartMs = 0;
    GraphNode* deleteNode = nullptr;

    TerminalDialog* terminal = nullptr;
    Terminal* terminal_ = nullptr;

    std::string selectedFileName_;
};

/**
 * @brief Глобальный указатель на активный GraphView
 */
extern GraphView* g_graphView;
