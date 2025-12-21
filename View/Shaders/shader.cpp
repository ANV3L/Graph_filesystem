#include "shader.hpp"
#include <QPainter>
#include <QPaintEvent>
#include <QtMath>


Shader::Shader(QWidget* parent) : QDialog(parent) {
    setAttribute(Qt::WA_TranslucentBackground, true);
    shimmerTimerId_ = startTimer(16);
}

void Shader::paintEvent(QPaintEvent* e) {
    Q_UNUSED(e);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const QRectF r0 = QRectF(rect());
    const QRectF r  = r0.adjusted(8, 8, -8, -8);
    const qreal rad = 16.0;

    {
        QColor g = accent_;
        for (int i = 0; i < 8; ++i) {
            const qreal k = i / 8.0;
            QRectF rr = r.adjusted(-10*k, -10*k, 10*k, 10*k);
            QColor c = g;
            c.setAlphaF(0.22 * (1.0 - k));
            p.setPen(QPen(c, 2.0));
            p.setBrush(Qt::NoBrush);
            p.drawRoundedRect(rr, rad + 6*k, rad + 6*k);
        }
    }

    {
        QColor sh(0, 0, 0, 190);
        for (int i = 0; i < 12; ++i) {
            qreal k = i / 12.0;
            QRectF rr = r.adjusted(-9*k, -9*k, 9*k, 9*k);
            QColor c = sh;
            c.setAlpha(int(34 * (1.0 - k)));
            p.setPen(QPen(c, 2.0));
            p.setBrush(Qt::NoBrush);
            p.drawRoundedRect(rr, rad + 6*k, rad + 6*k);
        }
    }

    {
        QLinearGradient bg(r.topLeft(), r.bottomRight());
        bg.setColorAt(0.00, QColor(7, 12, 34, 245));
        bg.setColorAt(0.45, QColor(3, 6, 18, 240));
        bg.setColorAt(1.00, QColor(0, 0, 10, 245));
        p.setPen(Qt::NoPen);
        p.setBrush(bg);
        p.drawRoundedRect(r, rad, rad);
    }

    {
        QColor a = accent_; a.setAlpha(155);
        QColor b = accent_; b.setAlpha(0);

        QRadialGradient rim(r.center(), r.width() * 0.95);
        rim.setColorAt(0.68, QColor(0,0,0,0));
        rim.setColorAt(0.90, a);
        rim.setColorAt(1.00, b);

        p.setPen(Qt::NoPen);
        p.setBrush(rim);
        p.drawRoundedRect(r, rad, rad);
    }

    {
        QRadialGradient gl(r.topLeft() + QPointF(110, 85), r.width() * 0.70);
        gl.setColorAt(0.00, QColor(255, 255, 255, 80));
        gl.setColorAt(0.40, QColor(255, 255, 255, 22));
        gl.setColorAt(1.00, QColor(255, 255, 255, 0));
        p.setBrush(gl);
        p.setPen(Qt::NoPen);
        p.drawRoundedRect(r, rad, rad);
    }

    {
        const qreal x = r.left() + (r.width() * (shimmer_ * 1.6) - r.width()*0.3);

        QLinearGradient sh(QPointF(x, r.top()), QPointF(x + r.width()*0.35, r.bottom()));
        QColor c1 = accent_; c1.setAlpha(0);
        QColor c2 = QColor(255, 255, 255, 0);
        QColor c3 = QColor(255, 255, 255, 70);
        QColor c4 = QColor(255, 255, 255, 0);

        sh.setColorAt(0.00, c1);
        sh.setColorAt(0.35, c2);
        sh.setColorAt(0.55, c3);
        sh.setColorAt(0.75, c4);
        sh.setColorAt(1.00, c1);

        p.setPen(Qt::NoPen);
        p.setBrush(sh);
        p.drawRoundedRect(r, rad, rad);
    }

    {
        p.save();
        p.setClipRect(r);

        QPen ln(QColor(255, 255, 255, 10));
        ln.setWidthF(1.0);
        p.setPen(ln);

        const int step = 4;
        for (int y = int(r.top()) + 2; y < int(r.bottom()); y += step) {
            p.drawLine(QPointF(r.left() + 10, y), QPointF(r.right() - 10, y));
        }

        p.restore();
    }

    {
        QColor inner(255, 255, 255, 28);
        p.setPen(QPen(inner, 1.0));
        p.setBrush(Qt::NoBrush);
        p.drawRoundedRect(r.adjusted(1.0, 1.0, -1.0, -1.0), rad - 1.0, rad - 1.0);
    }

    {
        QColor pen = accent_;
        pen.setAlpha(190);
        p.setPen(QPen(pen, 1.2));
        p.setBrush(Qt::NoBrush);
        p.drawRoundedRect(r, rad, rad);
    }
}


void Shader::timerEvent(QTimerEvent* e) {
    if (!e || e->timerId() != shimmerTimerId_) {
        QDialog::timerEvent(e);
        return;
    }

    shimmer_ += 0.012;
    if (shimmer_ >= 1.0) shimmer_ -= 1.0;
    update();
}

