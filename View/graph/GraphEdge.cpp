#include "GraphEdge.hpp"
#include "PhysicsNode.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QtMath>

GraphEdge::GraphEdge(PhysicsNode* a, PhysicsNode* b)
    : a(a), b(b) {

    setZValue(-2);
    setCacheMode(DeviceCoordinateCache);
    setFlag(QGraphicsItem::ItemHasNoContents, false);
    updatePath();
}

void GraphEdge::setHighlighted(bool on) {
    highlighted = on;
    update();
}

void GraphEdge::updatePath() {
    if (!a || !b) return;

    const QPointF p1 = a->pos();
    const QPointF p2 = b->pos();

    const QPointF d = (p2 - p1);
    const QPointF n(-d.y(), d.x());
    const double len = std::max(1.0, std::sqrt(d.x()*d.x() + d.y()*d.y()));
    const QPointF nn = n / len;

    const double bend = std::clamp(len * 0.12, 18.0, 55.0);
    const QPointF c = (p1 + p2) * 0.5 + nn * bend;

    QPainterPath path;
    path.moveTo(p1);
    path.quadTo(c, p2);
    setPath(path);

    prepareGeometryChange();
}

void GraphEdge::paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*) {
    if (!p) return;
    p->setRenderHint(QPainter::Antialiasing, true);

    const QPainterPath pp = path();

    {
        QPen glowPen(glowWhite);
        glowPen.setWidthF(6.8);
        glowPen.setCapStyle(Qt::RoundCap);
        glowPen.setJoinStyle(Qt::RoundJoin);
        p->setPen(glowPen);
        p->drawPath(pp);
    }

    {
        constexpr qreal k = 1.5;
        const qreal L = pp.length();
        const qreal base = std::fmod(L * 0.35, 1000.0);

        QPen ringPen(QColor(255, 255, 255, 150));
        ringPen.setCapStyle(Qt::RoundCap);
        ringPen.setJoinStyle(Qt::RoundJoin);
        ringPen.setDashPattern({2.0 * k, 7.0 * k});

        for (int i = 0; i < 3; ++i) {
            const qreal w = (2.8 - i * 0.7) * k;
            const int   a = 160 - i * 35;

            ringPen.setWidthF(std::max<qreal>(0.9, w));
            ringPen.setColor(QColor(255, 255, 255, std::max(20, a)));
            ringPen.setDashOffset(base + i * (2.6 * k));

            p->setPen(ringPen);
            p->drawPath(pp);
        }
    }

    {
        QColor c0 = parentColor;
        QColor c1 = childColor;
        c0.setAlpha(220);
        c1.setAlpha(220);

        QLinearGradient g(pp.pointAtPercent(0.0), pp.pointAtPercent(1.0));
        g.setColorAt(0.0, c0);
        g.setColorAt(1.0, c1);

        QPen corePen(QBrush(g), 2.3);
        corePen.setCapStyle(Qt::RoundCap);
        corePen.setJoinStyle(Qt::RoundJoin);
        p->setPen(corePen);
        p->drawPath(pp);
    }


    {
        QPen hiPen(QColor(255, 255, 255, 120));
        hiPen.setWidthF(1.0);
        hiPen.setCapStyle(Qt::RoundCap);
        p->setPen(hiPen);
        p->drawPath(pp);
    }
}
