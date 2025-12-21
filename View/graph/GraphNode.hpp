#pragma once

#include <string>
#include <vector>

#include <QPointF>
#include <QColor>

class QString;
class GraphEdge;
class QGraphicsScene;
class PhysicsNode;

/**
 * @brief Узел графа (каталог) для визуализации.
 */
class GraphNode {
public:
    /**
     * @brief Создаёт узел на сцене в указанной позиции с заданным именем и базовым цветом.
     */
    GraphNode(QGraphicsScene* scene, QPointF pos, const QString& name, const QColor& baseColor);

    /** @brief Графический объект узла (визуальный кружок). */
    PhysicsNode* circle = nullptr;

    /** @brief Ребро, соединяющее узел с родителем. */
    GraphEdge* connectionLine = nullptr;

    GraphNode* parent = nullptr;

    /** @brief Дочерние узлы (подкаталоги). */
    std::vector<GraphNode*> children;

    /** @brief Имена файлов, находящихся в каталоге узла. */
    std::vector<std::string> files;
};
