#pragma once

#include <QDialog>

#include "shader.hpp"

class QLineEdit;
class QPlainTextEdit;
class QString;
class Terminal;

/**
 * @brief Диалог терминала с вводом команд и выводом текста
 */
class TerminalDialog : public Shader {
    Q_OBJECT;
public:
    /**
     * @brief Создаёт диалог терминала
     * @param term Указатель на терминал или nullptr
     * @param parent Родительский виджет или nullptr
     */
    explicit TerminalDialog(Terminal* term, QWidget* parent = nullptr);

    /**
     * @brief Добавляет строку в область вывода
     * @param text Добавляемая строка
     */
    void appendText(const QString& text);

private:
    /**
     * @brief Обрабатывает Enter в поле ввода
     */
    void handleReturnPressed();

    Terminal* terminal_ = nullptr;
    QPlainTextEdit* output_ = nullptr;
    QLineEdit* input_ = nullptr;
};

/**
 * @brief Указатель на активный TerminalDialog
 */
extern TerminalDialog* g_terminal;

/**
 * @brief Печатает строку в терминал если он создан
 * @param text Печатаемая строка
 */
void terminalPrint(const QString& text);
