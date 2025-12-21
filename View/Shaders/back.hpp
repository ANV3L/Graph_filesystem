#pragma once
#include <QGraphicsScene>
#include <QPixmap>
#include <QRectF>

/**
 * @brief Сцена со звёздным фоном, который рисуется в drawBackground() через кэшируемый тайл и поддерживает параллакс.
 */
class StarfieldScene final : public QGraphicsScene {
    Q_OBJECT
public:
    /**
     * @brief Создаёт сцену звёздного фона.
     */
    explicit StarfieldScene(QObject* parent = nullptr);

    /**
     * @brief Устанавливает seed генерации фона и помечает кэш как устаревший.
     */
    void setSeed(quint32 s);

    /**
     * @brief Устанавливает коэффициент параллакса в диапазоне 0..1 для смещения тайлинга относительно камеры.
     */
    void setParallax(double k);

protected:
    /**
     * @brief Рисует фон: пересобирает кэш при необходимости и тайлит его по области отрисовки с учётом параллакса.
     */
    void drawBackground(QPainter* painter, const QRectF& rect) override;

private:
    /**
     * @brief Пересоздаёт кэш фона под заданный размер, если он устарел или изменился размер.
     */
    void rebuildCacheIfNeeded(const QSize& viewportSize);

private:
    quint32 seed_ = 1337;
    double parallax_ = 0.15;

    QSize cachedSize_;
    QPixmap cache_;
    bool dirty_ = true;
};
