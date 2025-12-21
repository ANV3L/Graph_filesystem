#include "OutputDialog.hpp"

#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QGraphicsDropShadowEffect>
#include <QColor>
#include <QPushButton>
#include <QHBoxLayout>

/**
 * @brief Создаёт диалог с заголовком и начальным текстом
 * @param title Заголовок окна
 * @param text Начальное содержимое
 * @param parent Родительский виджет или nullptr
 */
OutputDialog::OutputDialog(const QString& title,
                           const QString& text,
                           QWidget* parent)
    : Shader(parent),
      output_(nullptr)
{
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setModal(false);

    setFixedSize(700, 320);

    setWindowTitle(title);

    auto* glow = new QGraphicsDropShadowEffect(this);
    glow->setBlurRadius(55);
    glow->setOffset(0, 0);
    glow->setColor(QColor(120, 150, 255, 160));
    setGraphicsEffect(glow);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(8);

    output_ = new QPlainTextEdit(this);
    output_->setReadOnly(true);
    output_->setStyleSheet(
         "QPlainTextEdit {"
         "  background-color: #050814;"
         "  border: 1px solid #3A4AA0;"
         "  border-radius: 6px;"
         "  color: #E8EAED;"
         "  font: 10pt 'Consolas';"
         "}"
     );
    output_->setPlainText(text);
    layout->addWidget(output_);

    auto* btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    layout->addLayout(btnLayout);
}

/**
 * @brief Добавляет текст
 * @param text Добавляемая строка
 */
void OutputDialog::appendText(const QString& text) {
    if (!output_) return;
    output_->appendPlainText(text);
}

/**
 * @brief Создаёт диалог редактирования строки
 * @param title Заголовок окна
 * @param buffer Буфер для чтения и записи
 * @param parent Родительский виджет или nullptr
 */
TextEditorDialog::TextEditorDialog(const QString& title,
                                   std::string& buffer,
                                   QWidget* parent)
    : Shader(parent),
      buffer_(buffer),
      editor_(nullptr)
{
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setModal(false);

    setFixedSize(720, 380);

    setWindowTitle(title);

    setStyleSheet(
        "QDialog {"
        "  background-color: rgba(8, 12, 30, 235);"
        "  border: 1px solid #5C7CFF;"
        "  border-radius: 10px;"
        "}"
    );

    auto* glow = new QGraphicsDropShadowEffect(this);
    glow->setBlurRadius(32);
    glow->setOffset(0, 0);
    glow->setColor(QColor(140, 170, 255, 170));
    setGraphicsEffect(glow);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(6);

    editor_ = new QPlainTextEdit(this);
    editor_->setPlainText(QString::fromStdString(buffer_));
    editor_->setStyleSheet(
        "QPlainTextEdit {"
        "  background-color: #050814;"
        "  border: 1px solid #3A4AA0;"
        "  border-radius: 6px;"
        "  color: #E8F0FF;"
        "  font: 10pt 'Consolas';"
        "}"
    );
    layout->addWidget(editor_);

    auto* btnLayout = new QHBoxLayout;
    btnLayout->addStretch();

    auto* closeBtn = new QPushButton("Закрыть", this);

    auto* btnGlow = new QGraphicsDropShadowEffect(closeBtn);
    btnGlow->setBlurRadius(42);
    btnGlow->setOffset(0, 0);
    btnGlow->setColor(QColor(120, 150, 255, 120));
    closeBtn->setGraphicsEffect(btnGlow);


    closeBtn->setStyleSheet(
            "QPushButton {"
            "  padding: 6px 18px;"
            "  border-radius: 8px;"
            "  border: 1px solid rgba(140, 170, 255, 170);"
            "  color: rgb(240, 248, 255);"
            "  font: 10pt 'Segoe UI';"
            "  font-weight: 600;"
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
            "    stop:0 rgba(26, 37, 80, 220),"
            "    stop:1 rgba(10, 18, 50, 220));"
            "}"
            "QPushButton:hover {"
            "  border: 1px solid rgba(170, 200, 255, 220);"
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
            "    stop:0 rgba(34, 48, 100, 235),"
            "    stop:1 rgba(14, 24, 70, 235));"
            "}"
            "QPushButton:pressed {"
            "  padding-top: 7px;"
            "  padding-bottom: 5px;"
            "  border: 1px solid rgba(120, 150, 255, 200);"
            "  background: rgba(12, 18, 50, 235);"
            "}"
        );

    btnLayout->addWidget(closeBtn);
    layout->addLayout(btnLayout);

    connect(closeBtn, &QPushButton::clicked,
            this, &TextEditorDialog::onCloseClicked);
}

/**
 * @brief Сохраняет текст из редактора в буфер и закрывает диалог
 */
void TextEditorDialog::onCloseClicked() {
    buffer_ = editor_->toPlainText().toStdString();
    accept();
}
