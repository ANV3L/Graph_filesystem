#include "NameDialog.hpp"

#include <QColor>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

NameDialog::NameDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setModal(true);

    setFixedSize(430, 110);

    setWindowTitle(QString());

    setStyleSheet(
        "QDialog {"
        "  background-color: rgba(6, 10, 28, 242);"
        "  border: 1px solid rgba(140, 170, 255, 170);"
        "  border-radius: 14px;"
        "}"
    );

    auto* glow = new QGraphicsDropShadowEffect(this);
    glow->setBlurRadius(42);
    glow->setOffset(0, 0);
    glow->setColor(QColor(140, 170, 255, 165));
    setGraphicsEffect(glow);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(14, 12, 14, 12);
    layout->setSpacing(8);

    auto* title = new QLabel(this);
    title->setText(windowTitle().isEmpty() ? "Введите значение" : windowTitle());
    title->setStyleSheet(
        "QLabel {"
        "  color: rgb(240, 248, 255);"
        "  font: 600 10pt 'Segoe UI';"
        "  letter-spacing: 0.2px;"
        "}"
    );
    layout->addWidget(title);

    edit_ = new QLineEdit(this);
    edit_->setPlaceholderText("");
    edit_->setClearButtonEnabled(true);
    edit_->setStyleSheet(
        "QLineEdit {"
        "  background-color: rgba(5, 8, 20, 235);"
        "  border: 1px solid rgba(76, 108, 255, 170);"
        "  border-radius: 10px;"
        "  padding: 8px 12px;"
        "  color: rgb(240, 248, 255);"
        "  font: 11pt 'Consolas';"
        "  selection-background-color: rgba(34, 48, 100, 235);"
        "}"
        "QLineEdit:hover {"
        "  border: 1px solid rgba(140, 170, 255, 210);"
        "}"
        "QLineEdit:focus {"
        "  border: 1px solid rgba(170, 200, 255, 235);"
        "  background-color: rgba(8, 12, 30, 240);"
        "}"
    );

    auto* editGlow = new QGraphicsDropShadowEffect(edit_);
    editGlow->setBlurRadius(24);
    editGlow->setOffset(0, 0);
    editGlow->setColor(QColor(120, 150, 255, 120));
    edit_->setGraphicsEffect(editGlow);

    layout->addWidget(edit_);

    connect(edit_, &QLineEdit::returnPressed, this, &QDialog::accept);
    edit_->setFocus();
}

QString NameDialog::text() const {
    return edit_ ? edit_->text() : QString();
}
