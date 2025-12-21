#pragma once

#include <QDialog>
#include <QString>

#include "shader.hpp"

class QPlainTextEdit;

/**
 * @brief Диалог для вывода текста
 */
class OutputDialog : public Shader {
    Q_OBJECT;
public:
    /**
     * @brief Создаёт диалог с заголовком и начальным текстом
     * @param title Заголовок окна
     * @param text Начальное содержимое
     * @param parent Родительский виджет или nullptr
     */
    explicit OutputDialog(const QString& title,
                          const QString& text,
                          QWidget* parent = nullptr);

    /**
     * @brief Добавляет текст в конец вывода
     * @param text Добавляемая строка
     */
    void appendText(const QString& text);

private:
    QPlainTextEdit* output_ = nullptr;
};

#include <string>

class QPlainTextEdit;

/**
 * @brief Диалог редактирования текста в буфере
 */
class TextEditorDialog : public Shader {
    Q_OBJECT;
public:
    /**
     * @brief Создаёт диалог редактирования строки
     * @param title Заголовок окна
     * @param buffer Буфер для чтения и записи
     * @param parent Родительский виджет или nullptr
     */
    TextEditorDialog(const QString& title,
                     std::string& buffer,
                     QWidget* parent = nullptr);

private slots:
    /**
     * @brief Обработчик закрытия диалога
     */
    void onCloseClicked();

private:
    std::string& buffer_;
    QPlainTextEdit* editor_ = nullptr;
};
