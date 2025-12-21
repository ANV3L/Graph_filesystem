#pragma once

#include <vector>

#include <QPointF>
#include <QString>
#include <QColor>

class QGraphicsEllipseItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QGraphicsTextItem;

/**
 * @brief Круговое меню для выбора элементов с постраничной навигацией.
 */
class CircularMenu {
public:
    /**
     * @brief Создаёт меню, привязанное к заданной сцене.
     */
    explicit CircularMenu(QGraphicsScene* s);

    /**
     * @brief Устанавливает базовый цвет оформления меню.
     */
    void setThemeColor(const QColor& c);

    /**
     * @brief Обновляет центр меню; если меню показано, перерисовывает его.
     */
    void recenter(const QPointF& newCenter);

    /**
     * @brief Задаёт список элементов и сбрасывает выбор на первый элемент первой страницы.
     */
    void setItems(const std::vector<QString>& newItems);

    /**
     * @brief Показывает меню в указанной позиции (если список пуст — скрывает).
     */
    void show(QPointF pos);

    /**
     * @brief Скрывает меню и удаляет его графические элементы со сцены.
     */
    void hide();

    /**
     * @brief Возвращает количество страниц для текущего списка элементов.
     */
    int pagesCount() const;

    /**
     * @brief Переходит на следующую страницу и сбрасывает выбор на первый элемент.
     */
    void nextPage();

    /**
     * @brief Переходит на предыдущую страницу и сбрасывает выбор на первый элемент.
     */
    void prevPage();

    /**
     * @brief Выбирает следующий элемент на текущей странице.
     */
    void nextItem();

    /**
     * @brief Выбирает предыдущий элемент на текущей странице.
     */
    void prevItem();

    /**
     * @brief Возвращает индекс текущего выбранного элемента в общем списке (или -1).
     */
    int currentIndex() const;

    /**
     * @brief Возвращает текст текущего выбранного элемента (или пустую строку).
     */
    QString currentText() const;

    bool visible = false;

private:
    /**
     * @brief Перестраивает визуальные элементы меню согласно текущему состоянию (страница/выбор/цвет).
     */
    void updateMenu();

private:
    QGraphicsScene* scene = nullptr;
    QPointF center;

    std::vector<QString> items;
    int itemsPerPage = 6;
    int currentPage = 0;
    int currentItem = 0;

    std::vector<QGraphicsEllipseItem*> menuItems;
    QColor themeColor_ = QColor(180, 210, 255);
    std::vector<QGraphicsTextItem*> menuTexts;
    std::vector<QGraphicsLineItem*> rays;
};
