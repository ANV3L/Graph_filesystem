#include "back.hpp"

#include <QPainter>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QRandomGenerator>
#include <QtMath>

StarfieldScene::StarfieldScene(QObject* parent)
    : QGraphicsScene(parent)
{}

void StarfieldScene::setSeed(quint32 s) {
    seed_ = s;
    dirty_ = true;
    update();
}

void StarfieldScene::setParallax(double k) {
    parallax_ = std::clamp(k, 0.0, 1.0);
    update();
}

void StarfieldScene::rebuildCacheIfNeeded(const QSize& viewportSize) {
    if (!dirty_ && cachedSize_ == viewportSize && !cache_.isNull())
        return;

    cachedSize_ = viewportSize;
    cache_ = QPixmap(cachedSize_);
    cache_.fill(Qt::transparent);

    QPainter p(&cache_);
    p.setRenderHint(QPainter::Antialiasing, true);

    QRandomGenerator rng(seed_);

    {
        QLinearGradient g(0, 0, 0, cachedSize_.height());
        g.setColorAt(0.00, QColor(0, 0, 0));
        g.setColorAt(0.55, QColor(1, 1, 2));
        g.setColorAt(1.00, QColor(0, 0, 0));
        p.fillRect(QRect(QPoint(0, 0), cachedSize_), g);
    }

    {
        const int clouds = 14;
        for (int i = 0; i < clouds; ++i) {
            const double x = rng.generateDouble() * cachedSize_.width();
            const double y = rng.generateDouble() * cachedSize_.height();
            const double r = 260.0 + rng.generateDouble() * 620.0;

            QColor c;
            const int mode = int(rng.generateDouble() * 3.0);
            if (mode == 0) c = QColor(18, 28, 80);
            else if (mode == 1) c = QColor(45, 18, 75);
            else c = QColor(12, 40, 55);

            const int a = 2 + int(rng.generateDouble() * 7.0);
            c.setAlpha(a);

            QColor cMid = c; cMid.setAlpha(int(a * 0.40));
            QColor cOut = c; cOut.setAlpha(0);

            QRadialGradient neb(QPointF(x, y), r);
            neb.setColorAt(0.00, c);
            neb.setColorAt(0.55, cMid);
            neb.setColorAt(1.00, cOut);

            p.setPen(Qt::NoPen);
            p.setBrush(neb);
            p.drawEllipse(QRectF(x - r, y - r, 2 * r, 2 * r));
        }
    }

    {
        p.setPen(Qt::NoPen);

        const int dustFine = 520;
        for (int i = 0; i < dustFine; ++i) {
            const int x = int(rng.generateDouble() * cachedSize_.width());
            const int y = int(rng.generateDouble() * cachedSize_.height());
            const int a = 1 + int(rng.generateDouble() * 6.0);
            p.setBrush(QColor(255, 255, 255, a));
            p.drawEllipse(QRectF(x, y, 1, 1));
        }

        const int dustBig = 90;
        for (int i = 0; i < dustBig; ++i) {
            const int x = int(rng.generateDouble() * cachedSize_.width());
            const int y = int(rng.generateDouble() * cachedSize_.height());
            const int a = 4 + int(rng.generateDouble() * 10.0);
            const int s = 1 + int(rng.generateDouble() * 2.0);
            p.setBrush(QColor(255, 255, 255, a));
            p.drawEllipse(QRectF(x, y, s, s));
        }
    }

    {
        auto drawStar = [&](double x, double y, double r, int alpha, const QColor& tint, bool haloOn) {
            if (haloOn) {
                QColor halo = tint;
                halo.setAlpha(std::max(0, alpha / 10));

                QRadialGradient h(QPointF(x, y), r * 6.5);
                h.setColorAt(0.00, halo);
                h.setColorAt(1.00, QColor(halo.red(), halo.green(), halo.blue(), 0));
                p.setPen(Qt::NoPen);
                p.setBrush(h);
                p.drawEllipse(QRectF(x - r * 6.5, y - r * 6.5, r * 13.0, r * 13.0));
            }

            QColor core = tint;
            core.setAlpha(alpha);
            p.setPen(Qt::NoPen);
            p.setBrush(core);
            p.drawEllipse(QRectF(x - r, y - r, 2 * r, 2 * r));
        };

        const int faint = 190;
        for (int i = 0; i < faint; ++i) {
            const double x = rng.generateDouble() * cachedSize_.width();
            const double y = rng.generateDouble() * cachedSize_.height();

            const double r = 0.45 + rng.generateDouble() * 0.75;
            const int a = 14 + int(rng.generateDouble() * 42.0);

            QColor tint(255, 255, 255);
            const double t = rng.generateDouble();
            if (t < 0.10) tint = QColor(170, 200, 255);
            else if (t < 0.14) tint = QColor(255, 220, 170);

            drawStar(x, y, r, a, tint, false);
        }

        const int bright = 26;
        for (int i = 0; i < bright; ++i) {
            const double x = rng.generateDouble() * cachedSize_.width();
            const double y = rng.generateDouble() * cachedSize_.height();

            const double r = 0.9 + rng.generateDouble() * 1.6;
            const int a = 90 + int(rng.generateDouble() * 110.0);

            QColor tint(255, 255, 255);
            const double t = rng.generateDouble();
            if (t < 0.20) tint = QColor(180, 210, 255);
            else if (t < 0.28) tint = QColor(255, 220, 170);

            drawStar(x, y, r, a, tint, true);

            if (rng.generateDouble() < 0.03) {
                QPen pen(QColor(255, 255, 255, a / 5));
                pen.setWidthF(1.0);
                pen.setCapStyle(Qt::RoundCap);
                p.setPen(pen);
                p.drawLine(QPointF(x - 9, y), QPointF(x + 9, y));
                p.drawLine(QPointF(x, y - 9), QPointF(x, y + 9));
            }
        }
    }

    {
        const QPointF c(cachedSize_.width() * 0.5, cachedSize_.height() * 0.55);
        const double R = std::hypot(cachedSize_.width(), cachedSize_.height()) * 0.70;

        QRadialGradient v(c, R);
        v.setColorAt(0.00, QColor(0, 0, 0, 0));
        v.setColorAt(0.60, QColor(0, 0, 0, 25));
        v.setColorAt(0.82, QColor(0, 0, 0, 140));
        v.setColorAt(1.00, QColor(0, 0, 0, 235));

        p.setPen(Qt::NoPen);
        p.setBrush(v);
        p.drawRect(QRect(QPoint(0, 0), cachedSize_));
    }

    dirty_ = false;
}


void StarfieldScene::drawBackground(QPainter* painter, const QRectF& rect) {
    if (!painter) return;

    const int w = int(std::clamp(rect.width(),  400.0, 2200.0));
    const int h = int(std::clamp(rect.height(), 300.0, 1400.0));

    rebuildCacheIfNeeded(QSize(w, h));

    const QPointF shift = rect.topLeft() * parallax_;
    const int ox = int(std::fmod(-shift.x(), double(cache_.width())));
    const int oy = int(std::fmod(-shift.y(), double(cache_.height())));

    for (int y = oy - cache_.height(); y < h + cache_.height(); y += cache_.height()) {
        for (int x = ox - cache_.width(); x < w + cache_.width(); x += cache_.width()) {
            painter->drawPixmap(QPointF(rect.left() + x, rect.top() + y), cache_);
        }
    }
}
