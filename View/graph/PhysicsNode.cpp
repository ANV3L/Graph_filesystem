#include "PhysicsNode.hpp"

#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QTimerEvent>

#include <algorithm>
#include <cmath>

static QColor mixColor(const QColor &a, const QColor &b, double t) {
    t = std::clamp(t, 0.0, 1.0);
    return QColor(
        int(a.red() + (b.red() - a.red()) * t),
        int(a.green() + (b.green() - a.green()) * t),
        int(a.blue() + (b.blue() - a.blue()) * t),
        int(a.alpha() + (b.alpha() - a.alpha()) * t)
    );
}

PhysicsNode::PhysicsNode(QGraphicsScene *scene, QPointF pos, const QString &nodeName)
    : name(nodeName) {
    setPos(pos);
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(1);

    label = scene->addText(name, QFont("Segoe UI", 10, QFont::DemiBold));
    label->setDefaultTextColor(QColor(240, 248, 255));
    label->setZValue(2);

    auto *textShadow = new QGraphicsDropShadowEffect();
    textShadow->setBlurRadius(12);
    textShadow->setOffset(0, 0);
    textShadow->setColor(QColor(0, 0, 0, 180));
    label->setGraphicsEffect(textShadow);


    initEffects_();
    updateLabelPosition();
    setToolTip(name);
}

void PhysicsNode::initEffects_() {
    shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(26);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor(0, 0, 0, 160));
    setGraphicsEffect(shadow);
}

void PhysicsNode::updateLabelPosition() {
    if (!label) return;
    const QPointF p = pos();
    label->setPos(p.x() - label->boundingRect().width() / 2.0, p.y() + 30.0);
}

void PhysicsNode::setBaseColor(const QColor &c) {
    baseColor_ = c;

    update();
}

void PhysicsNode::setCurrent(bool isCurrent) {
    if (deleting) return;

    isCurrent_ = isCurrent;

    if (shadow) {
        shadow->setBlurRadius(isCurrent_ ? 55 : 26);

        if (isCurrent_) {
            shadow->setColor(QColor(255, 245, 210, 220));
        } else {
            shadow->setColor(QColor(0, 0, 0, 160));
        }
    }

    if (isCurrent_) startShimmer_();
    else stopShimmer_();

    update();
}

void PhysicsNode::setDeleteProgress(double t) {
    deleting = (t > 0.0);
    deleteProgress = std::clamp(t, 0.0, 1.0);

    if (shadow) {
        shadow->setBlurRadius(30 + 25 * deleteProgress);
        shadow->setColor(QColor(255, 60, 70, int(160 + 80 * deleteProgress)));
    }

    update();
}

QRectF PhysicsNode::boundingRect() const {
    const qreal pad = isCurrent_ ? 36.0 : 28.0;
    return QRectF(-radius_ - pad, -radius_ - pad, 2 * (radius_ + pad), 2 * (radius_ + pad));
}

QPainterPath PhysicsNode::shape() const {
    QPainterPath p;
    p.addEllipse(QRectF(-radius_, -radius_, 2 * radius_, 2 * radius_));
    return p;
}

QVariant PhysicsNode::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionHasChanged) {
        updateLabelPosition();
    }
    return QGraphicsObject::itemChange(change, value);
}

void PhysicsNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    if (!painter) return;
    painter->setRenderHint(QPainter::Antialiasing, true);

    const double phase = t_;

    const QColor blue(180, 210, 255);
    const QColor blueDeep(90, 120, 180);
    const QColor white(255, 255, 255);
    const QColor warm(255, 255, 255);


    QColor c0 = baseColor_;
    if (isCurrent_) {
        const double k = 0.5 + 0.5 * std::sin(phase * 1.6);
        c0 = mixColor(baseColor_, QColor(255, 255, 255), 0.15 + 0.20 * k);
    }


    QColor core = mixColor(white, c0, 0.35);
    QColor mid = mixColor(c0, mixColor(c0, white, 0.25), 0.6);
    QColor edge = mixColor(QColor(10, 15, 30), c0, isCurrent_ ? 0.35 : 0.25);

    if (deleting) {
        const QColor hot(255, 70, 70);
        core = mixColor(core, hot, deleteProgress);
        mid = mixColor(mid, hot, deleteProgress);
        edge = mixColor(edge, hot, deleteProgress);
    }

    QRadialGradient body(QPointF(-6, -8), radius_ * 1.25);
    body.setColorAt(0.00, core);
    body.setColorAt(0.40, mid);
    body.setColorAt(1.00, edge);

    painter->setPen(Qt::NoPen);
    painter->setBrush(body);
    painter->drawEllipse(QRectF(-radius_, -radius_, 2 * radius_, 2 * radius_));

    QColor penCol;
    if (isCurrent_) {
        const double k = 0.5 + 0.5 * std::sin(phase * 1.6);
        penCol = mixColor(baseColor_, QColor(255, 255, 255), 0.35 + 0.20 * k);
    } else {
        penCol = mixColor(baseColor_, QColor(255, 255, 255), 0.20);
    }


    if (deleting) penCol = mixColor(penCol, QColor(255, 120, 120), deleteProgress);

    painter->setPen(QPen(penCol, isCurrent_ ? 3.4 : 2.0));
    painter->setBrush(Qt::NoBrush);
    painter->drawEllipse(QRectF(-radius_, -radius_, 2 * radius_, 2 * radius_));

    QRadialGradient rim(QPointF(0, 0), radius_ * 1.12);
    rim.setColorAt(0.70, QColor(0, 0, 0, 0));
    rim.setColorAt(0.86, QColor(penCol.red(), penCol.green(), penCol.blue(), isCurrent_ ? 150 : 70));
    rim.setColorAt(1.00, QColor(255, 255, 255, 0));

    painter->setPen(Qt::NoPen);
    painter->setBrush(rim);
    painter->drawEllipse(QRectF(-radius_ * 1.06, -radius_ * 1.06, 2 * radius_ * 1.06, 2 * radius_ * 1.06));

    if (isCurrent_) {
        auto drawGlint = [&](QPointF p, qreal r, const QColor &col) {
            QRadialGradient g(p, r);
            g.setColorAt(0.0, QColor(col.red(), col.green(), col.blue(), 190));
            g.setColorAt(1.0, QColor(col.red(), col.green(), col.blue(), 0));
            painter->setBrush(g);
            painter->drawEllipse(QRectF(p.x() - r, p.y() - r, 2 * r, 2 * r));
        };

        drawGlint(QPointF(-10 + 6 * std::sin(phase * 1.3), -12 + 5 * std::cos(phase * 1.1)),
                  16, white);

        drawGlint(QPointF(10 + 7 * std::cos(phase * 0.9), 8 + 6 * std::sin(phase * 1.05)),
                  14, warm);
    }


    if (isCurrent_ && !deleting) {
        painter->setPen(Qt::NoPen);

        const double pulse = 0.5 + 0.5 * std::sin(phase * 1.2);

        QColor auraInnerCol = mixColor(baseColor_, QColor(255, 255, 255), 0.25 + 0.10 * pulse);
        QColor auraMidCol = mixColor(baseColor_, QColor(255, 255, 255), 0.45 + 0.10 * pulse);
        QColor auraOuterCol = mixColor(baseColor_, QColor(255, 255, 255), 0.55 + 0.10 * pulse);

        QColor auraInner(auraInnerCol.red(), auraInnerCol.green(), auraInnerCol.blue(),
                         int(90 + 50 * pulse));
        QColor auraMid(auraMidCol.red(), auraMidCol.green(), auraMidCol.blue(),
                       int(70 + 35 * (1.0 - pulse)));
        QColor auraOuter(auraOuterCol.red(), auraOuterCol.green(), auraOuterCol.blue(),
                         int(35 + 25 * pulse));

        QRadialGradient aura(QPointF(0, 0), radius_ * 2.05);
        aura.setColorAt(0.00, QColor(0, 0, 0, 0));
        aura.setColorAt(0.55, QColor(0, 0, 0, 0));
        aura.setColorAt(0.72, auraInner);
        aura.setColorAt(0.88, auraMid);
        aura.setColorAt(1.00, QColor(auraOuter.red(), auraOuter.green(), auraOuter.blue(), 0));

        painter->setBrush(aura);
        const qreal rr = radius_ * (1.55 + 0.10 * pulse);
        painter->drawEllipse(QRectF(-rr, -rr, 2 * rr, 2 * rr));
    }
}

void PhysicsNode::startShimmer_() {
    if (shimmerTimerId_ != 0) return;
    shimmerTimerId_ = startTimer(16);
}

void PhysicsNode::stopShimmer_() {
    if (shimmerTimerId_ == 0) return;
    killTimer(shimmerTimerId_);
    shimmerTimerId_ = 0;
}

void PhysicsNode::timerEvent(QTimerEvent *event) {
    if (!event) return;

    if (event->timerId() == shimmerTimerId_) {
        t_ += shimmerSpeed_;
        update();
        return;
    }

    QGraphicsObject::timerEvent(event);
}
