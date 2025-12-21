#include "GraphNode.hpp"
#include "PhysicsNode.hpp"

#include "GraphEdge.hpp"
#include <QGraphicsScene>
#include <QColor>

static QColor colorFromName(const QString& s) {
    uint h = 2166136261u;
    for (auto ch : s) {
        h ^= ch.unicode();
        h *= 16777619u;
    }

    const int hue = int(h % 360u);

    QColor c;
    c.setHsv(hue, 160, 230);
    return c;
}

GraphNode::GraphNode(QGraphicsScene* scene, QPointF pos, const QString& name, const QColor& baseColor) {
    circle = new PhysicsNode(scene, pos, name);
    circle->setBaseColor(baseColor);
    scene->addItem(circle);
}
