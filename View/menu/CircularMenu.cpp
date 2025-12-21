#include "CircularMenu.hpp"

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsLineItem>
#include <QGraphicsDropShadowEffect>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QtMath>
#include <algorithm>

CircularMenu::CircularMenu(QGraphicsScene* s)
    : scene(s)
{}

void CircularMenu::recenter(const QPointF& newCenter) {
    center = newCenter;
    if (visible) {
        updateMenu();
    }
}

void CircularMenu::setItems(const std::vector<QString>& newItems) {
    items = newItems;
    currentPage = 0;
    currentItem = 0;
}

void CircularMenu::setThemeColor(const QColor& c) {
    themeColor_ = c;
    if (visible) updateMenu();
}


void CircularMenu::show(QPointF pos) {
    if (items.empty()) {
        hide();
        return;
    }
    center  = pos;
    visible = true;
    updateMenu();
}

void CircularMenu::hide() {
    for (auto item : menuItems) scene->removeItem(item);
    for (auto text : menuTexts) scene->removeItem(text);
    for (auto ray  : rays)      scene->removeItem(ray);
    menuItems.clear();
    menuTexts.clear();
    rays.clear();
    visible = false;
}

int CircularMenu::pagesCount() const {
    if (items.empty()) return 0;
    return int((items.size() + itemsPerPage - 1) / itemsPerPage);
}

void CircularMenu::nextPage() {
    int pc = pagesCount();
    if (pc == 0) return;
    currentPage = (currentPage + 1) % pc;
    currentItem = 0;
    updateMenu();
}

void CircularMenu::prevPage() {
    int pc = pagesCount();
    if (pc == 0) return;
    currentPage = (currentPage + pc - 1) % pc;
    currentItem = 0;
    updateMenu();
}

void CircularMenu::nextItem() {
    int pc = pagesCount();
    if (pc == 0) return;
    int start = currentPage * itemsPerPage;
    int count = std::min(itemsPerPage, int(items.size()) - start);
    if (count <= 0) return;
    currentItem = (currentItem + 1) % count;
    updateMenu();
}

void CircularMenu::prevItem() {
    int pc = pagesCount();
    if (pc == 0) return;
    int start = currentPage * itemsPerPage;
    int count = std::min(itemsPerPage, int(items.size()) - start);
    if (count <= 0) return;
    currentItem = (currentItem + count - 1) % count;
    updateMenu();
}

int CircularMenu::currentIndex() const {
    int pc = pagesCount();
    if (pc == 0) return -1;
    int start = currentPage * itemsPerPage;
    int idx   = start + currentItem;
    if (idx < 0 || idx >= int(items.size())) return -1;
    return idx;
}

QString CircularMenu::currentText() const {
    int idx = currentIndex();
    if (idx < 0) return {};
    return items[idx];
}


void CircularMenu::updateMenu() {
    for (auto* item : menuItems) scene->removeItem(item);
    for (auto* text : menuTexts) scene->removeItem(text);
    for (auto* ray  : rays)      scene->removeItem(ray);

    menuItems.clear();
    menuTexts.clear();
    rays.clear();

    const int pc = pagesCount();
    if (pc <= 0) { visible = false; return; }

    const int start = currentPage * itemsPerPage;
    const int count = std::min(itemsPerPage, int(items.size()) - start);
    if (count <= 0) { visible = false; return; }

    auto mix = [](QColor a, QColor b, double t) -> QColor {
        t = std::clamp(t, 0.0, 1.0);
        return QColor(int(a.red()   + (b.red()   - a.red())   * t),
                      int(a.green() + (b.green() - a.green()) * t),
                      int(a.blue()  + (b.blue()  - a.blue())  * t),
                      int(a.alpha() + (b.alpha() - a.alpha()) * t));
    };

    const qreal ringRadius = 140.0;
    const qreal nodeRadius = 28.0;
    const qreal angleStep  = 360.0 / count;

    QColor theme = themeColor_;
    if (!theme.isValid()) theme = QColor(180, 210, 255);

    QFont font("Consolas", 11);
    font.setStyleHint(QFont::Monospace);

    const qreal rBtn     = 22.0;
    const qreal rAura    = 40.0;
    const qreal rRim     = 26.0;
    const qreal rGlint   = 12.0;

    for (int i = 0; i < count; ++i) {
        const bool isCurrent = (i == currentItem);

        const qreal angle = (i * angleStep - 90.0) * M_PI / 180.0;
        const qreal x = center.x() + ringRadius * std::cos(angle);
        const qreal y = center.y() + ringRadius * std::sin(angle);

        const qreal sx = center.x() + nodeRadius * std::cos(angle);
        const qreal sy = center.y() + nodeRadius * std::sin(angle);

        {
            QColor rayCol = theme;
            rayCol.setAlpha(isCurrent ? 210 : 120);
            QPen rayPen(rayCol, isCurrent ? 2.2 : 1.4, Qt::SolidLine, Qt::RoundCap);
            auto* ray = scene->addLine(sx, sy, x, y, rayPen);
            ray->setZValue(6);
            rays.push_back(ray);
        }

        QColor base = theme; base.setAlpha(255);

        const double kSel = isCurrent ? 1.0 : 0.0;

        QColor core = mix(QColor(255,255,255), base, 0.35);
        core = mix(core, QColor(255,255,255), 0.15 * kSel);

        QColor mid  = mix(base, QColor(255,255,255), 0.10 + 0.10 * kSel);
        QColor edge = mix(QColor(10,15,30), base, 0.45);
        edge = mix(edge, QColor(0,0,0), isCurrent ? 0.05 : 0.18);

        {
            QColor auraCol = theme;
            auraCol.setAlpha(isCurrent ? 120 : 70);

            QRadialGradient aura(0, 0, rAura);
            aura.setColorAt(0.00, QColor(0,0,0,0));
            aura.setColorAt(0.55, QColor(0,0,0,0));
            aura.setColorAt(0.72, QColor(auraCol.red(), auraCol.green(), auraCol.blue(), auraCol.alpha()));
            aura.setColorAt(1.00, QColor(auraCol.red(), auraCol.green(), auraCol.blue(), 0));

            auto* auraItem = scene->addEllipse(-rAura, -rAura, 2*rAura, 2*rAura,
                                               QPen(Qt::NoPen), QBrush(aura));
            auraItem->setPos(x, y);
            auraItem->setZValue(7);
            menuItems.push_back(auraItem);
        }

        {
            QRadialGradient body(-6, -8, rBtn * 1.25);
            body.setColorAt(0.00, core);
            body.setColorAt(0.40, mid);
            body.setColorAt(1.00, edge);

            QColor penCol = mix(base, QColor(255,255,255), isCurrent ? 0.30 : 0.18);
            penCol.setAlpha(isCurrent ? 230 : 150);

            auto* button = scene->addEllipse(-rBtn, -rBtn, 2*rBtn, 2*rBtn,
                                             QPen(penCol, isCurrent ? 2.6 : 1.6),
                                             QBrush(body));
            button->setPos(x, y);
            button->setZValue(8);
            menuItems.push_back(button);
        }

        {
            QColor rimCol = mix(base, QColor(255,255,255), isCurrent ? 0.35 : 0.18);
            rimCol.setAlpha(isCurrent ? 170 : 90);

            QRadialGradient rim(0, 0, rRim * 1.10);
            rim.setColorAt(0.70, QColor(0,0,0,0));
            rim.setColorAt(0.86, rimCol);
            rim.setColorAt(1.00, QColor(rimCol.red(), rimCol.green(), rimCol.blue(), 0));

            auto* rimItem = scene->addEllipse(-rRim, -rRim, 2*rRim, 2*rRim,
                                              QPen(Qt::NoPen), QBrush(rim));
            rimItem->setPos(x, y);
            rimItem->setZValue(9);
            menuItems.push_back(rimItem);
        }

        {
            QColor gl = QColor(255,255,255, isCurrent ? 170 : 120);
            QColor glOut = QColor(255,255,255, 0);

            QRadialGradient g(-8, -10, rGlint);
            g.setColorAt(0.00, gl);
            g.setColorAt(1.00, glOut);

            const qreal rr = rGlint;
            auto* glItem = scene->addEllipse(-rr, -rr, 2*rr, 2*rr,
                                             QPen(Qt::NoPen), QBrush(g));
            glItem->setPos(x, y);
            glItem->setZValue(10);
            menuItems.push_back(glItem);
        }

        {
            auto* text = scene->addText(items[start + i], QFont("Segoe UI", 10, QFont::DemiBold));
            text->setDefaultTextColor(QColor(240, 248, 255));
            text->setZValue(11);

            QRectF br = text->boundingRect();
            text->setPos(x - br.width() / 2.0, y - br.height() / 2.0);

            auto* textShadow = new QGraphicsDropShadowEffect(text);
            textShadow->setBlurRadius(12);
            textShadow->setOffset(0, 0);
            textShadow->setColor(QColor(0, 0, 0, 180));
            text->setGraphicsEffect(textShadow);

            menuTexts.push_back(text);
        }

    }
}
