#pragma once
#include <QGraphicsPathItem>
#include <QPointer>
#include <QColor>

class PhysicsNode;

/**
 * @brief Графическое ребро между двумя узлами (PhysicsNode) в QGraphicsScene, рисуемое как изогнутая светящаяся линия с градиентом от parent к child.
 */
class GraphEdge final : public QGraphicsPathItem {
public:
    /**
     * @brief Создаёт ребро между двумя узлами и инициализирует параметры отрисовки/кэширования, после чего строит начальный путь.
     */
    GraphEdge(PhysicsNode* a, PhysicsNode* b);

    /**
     * @brief Пересчитывает геометрию кривой (QPainterPath) по текущим позициям узлов.
     */
    void updatePath();

    /**
     * @brief Включает или выключает режим подсветки ребра (визуальный акцент) и инициирует перерисовку.
     */
    void setHighlighted(bool on);

    /**
     * @brief Устанавливает базовый цвет “ядра” ребра (может использоваться как общий тон отрисовки).
     */
    void setCoreColor(const QColor& c) { coreColor = c; }

    /**
     * @brief Устанавливает цвета концов ребра (parent -> child) для градиента и инициирует перерисовку.
     */
    void setEndpointColors(const QColor& parent, const QColor& child) {
        parentColor = parent;
        childColor = child;
        update();
    }

protected:
    /**
     * @brief Отрисовывает ребро (glow, “кольца/штрихи”, градиентное ядро и тонкий highlight) поверх рассчитанного пути.
     */
    void paint(QPainter* p, const QStyleOptionGraphicsItem* opt, QWidget* w) override;

private:
    QPointer<PhysicsNode> a;
    QPointer<PhysicsNode> b;
    bool highlighted = false;

    QColor coreColor = QColor(180, 210, 255);
    QColor glowWhite = QColor(255, 255, 255, 55);

    QColor parentColor = QColor(180, 210, 255);
    QColor childColor  = QColor(180, 210, 255);
};
