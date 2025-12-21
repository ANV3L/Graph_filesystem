#pragma once

#include <QGraphicsObject>
#include <QPointF>
#include <QColor>
#include <QString>
#include <QGraphicsTextItem>

class QGraphicsDropShadowEffect;
class QGraphicsScene;
class QTimerEvent;

/**
 * @brief Графический узел (круг) для QGraphicsScene с подписью, подсветкой текущего узла и анимацией удаления.
 */
class PhysicsNode final : public QGraphicsObject {
public:
    /**
     * @brief Создаёт узел на сцене в заданной позиции и создаёт текстовую метку с именем.
     */
    explicit PhysicsNode(QGraphicsScene* scene, QPointF pos, const QString& nodeName);

    ~PhysicsNode() override = default;

    /**
     * @brief Обновляет позицию текстовой метки относительно текущей позиции узла.
     */
    void updateLabelPosition();

    /**
     * @brief Включает или выключает режим текущего узла (подсветка и shimmer-анимация).
     */
    void setCurrent(bool isCurrent);

    /**
     * @brief Устанавливает прогресс удаления узла (0..1) и обновляет визуальные эффекты.
     */
    void setDeleteProgress(double t);

    /**
     * @brief Устанавливает базовый цвет узла.
     */
    void setBaseColor(const QColor& c);

    /**
     * @brief Возвращает базовый цвет узла.
     */
    [[nodiscard]] QColor baseColor() const { return baseColor_; }

    /** @brief Скорость узла в “физике” GraphView. */
    QPointF velocity{0, 0};

    /** @brief Суммарная сила, действующая на узел в “физике” GraphView. */
    QPointF force{0, 0};

    /** @brief Имя узла (используется как текст метки). */
    QString name;

    /** @brief Текстовая метка узла на сцене. */
    QGraphicsTextItem* label = nullptr;

    /** @brief Drop shadow эффект, используемый для подсветки/состояний. */
    QGraphicsDropShadowEffect* shadow = nullptr;

    /** @brief Флаг состояния удаления (true, если запущена анимация удаления). */
    bool deleting = false;

    /** @brief Прогресс удаления в диапазоне 0..1. */
    double deleteProgress = 0.0;

    /**
     * @brief Возвращает bounding rect с учётом визуальных эффектов (glow/aura).
     */
    [[nodiscard]] QRectF boundingRect() const override;

    /**
     * @brief Возвращает форму узла для hit-test/коллизий.
     */
    [[nodiscard]] QPainterPath shape() const override;

    /**
     * @brief Рисует узел (тело, контур, блики, aura для current и эффекты удаления).
     */
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

protected:
    /**
     * @brief Обрабатывает изменения элемента; при перемещении обновляет позицию метки.
     */
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

    /**
     * @brief Обрабатывает события таймера shimmer-анимации.
     */
    void timerEvent(QTimerEvent* event) override;

private:
    /**
     * @brief Инициализирует визуальные эффекты (shadow) для узла.
     */
    void initEffects_();

    /**
     * @brief Запускает shimmer-анимацию узла.
     */
    void startShimmer_();

    /**
     * @brief Останавливает shimmer-анимацию узла.
     */
    void stopShimmer_();

private:
    bool isCurrent_ = false;
    QColor baseColor_{180, 200, 255};
    qreal radius_ = 25.0;

    int shimmerTimerId_ = 0;
    double t_ = 0.0;
    double shimmerSpeed_ = 0.055;
};
