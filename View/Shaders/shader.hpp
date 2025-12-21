#pragma once
#include <QDialog>
#include <QColor>

class QPaintEvent;
class QTimerEvent;

/**
 * @brief Базовый диалог с кастомной отрисовкой “стеклянной” панели и анимированным shimmer-эффектом, настраиваемым акцентным цветом.
 */
class Shader : public QDialog {
    Q_OBJECT
public:
    /**
     * @brief Создаёт диалог и включает прозрачный фон для кастомной отрисовки.
     */
    explicit Shader(QWidget* parent = nullptr);

    /**
     * @brief Устанавливает акцентный цвет оформления и инициирует перерисовку.
     */
    void setAccentColor(const QColor& c) { accent_ = c; update(); }

    /**
     * @brief Возвращает текущий акцентный цвет.
     */
    QColor accentColor() const { return accent_; }

protected:
    /**
     * @brief Рисует фон и эффекты (glow, rim, блик, shimmer) внутри формы диалога.
     */
    void paintEvent(QPaintEvent* e) override;

    /**
     * @brief Обрабатывает таймер shimmer-анимации и инициирует перерисовку.
     */
    void timerEvent(QTimerEvent* e) override;

private:
    QColor accent_{120, 160, 255};
    int shimmerTimerId_ = 0;
    double shimmer_ = 0.0;
};
