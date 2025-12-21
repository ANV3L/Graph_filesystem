#pragma once

#include <QDialog>

class QLineEdit;

/**
 * @brief Диалог ввода строки пользователем
 */
class NameDialog : public QDialog {
    Q_OBJECT;
public:
    /**
     * @brief Создаёт модальный диалог ввода
     * @param parent Родительский виджет или nullptr
     */
    explicit NameDialog(QWidget* parent = nullptr);

    /**
     * @brief Возвращает введённый текст
     * @return Текст из поля ввода или пустая строка если поле не создано
     */
    QString text() const;

private:
    QLineEdit* edit_ = nullptr;
};
