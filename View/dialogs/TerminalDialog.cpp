#include "TerminalDialog.hpp"

#include "../Controller/Terminal/terminal.hpp"
#include "GraphView.hpp"
#include "PhysicsNode.hpp"

#include <QApplication>
#include <QColor>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <fstream>
#include <unordered_map>

TerminalDialog* g_terminal = nullptr;

/**
 * @brief Создаёт диалог терминала
 * @param term Указатель на терминал или nullptr
 * @param parent Родительский виджет или nullptr
 */
TerminalDialog::TerminalDialog(Terminal* term, QWidget* parent)
    : Shader(parent), terminal_(term)
{
    g_terminal = this;

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setModal(false);

    setFixedSize(720, 360);

    setAccentColor(QColor(120, 150, 255));
    setWindowTitle(QString());

    auto* glow = new QGraphicsDropShadowEffect(this);
    glow->setBlurRadius(38);
    glow->setOffset(0, 0);
    glow->setColor(QColor(120, 150, 255, 170));
    setGraphicsEffect(glow);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    auto* title = new QLabel("Терминал", this);
    title->setStyleSheet(
        "QLabel {"
        "  color: rgb(240, 248, 255);"
        "  font: 600 11pt 'Segoe UI';"
        "  letter-spacing: 0.3px;"
        "}"
    );
    layout->addWidget(title);

    output_ = new QPlainTextEdit(this);
    output_->setReadOnly(true);
    output_->setFrameStyle(QFrame::NoFrame);
    output_->setStyleSheet(
        "QPlainTextEdit {"
        "  background-color: rgba(5, 8, 20, 235);"
        "  border: 1px solid rgba(58, 74, 160, 190);"
        "  border-radius: 8px;"
        "  color: rgb(240, 248, 255);"
        "  font: 10 pt 'Consolas';"
        "  selection-background-color: rgba(34, 48, 100, 235);"
        "  selection-color: rgb(240, 248, 255);"
        "}"
        "QPlainTextEdit:focus {"
        "  border: 1px solid rgba(170, 200, 255, 210);"
        "}"
    );

    auto* outGlow = new QGraphicsDropShadowEffect(output_);
    outGlow->setBlurRadius(26);
    outGlow->setOffset(0, 0);
    outGlow->setColor(QColor(120, 150, 255, 75));
    output_->setGraphicsEffect(outGlow);

    layout->addWidget(output_, 1);

    auto* bottom = new QHBoxLayout;
    bottom->setSpacing(10);

    input_ = new QLineEdit(this);
    input_->setPlaceholderText("");
    input_->setClearButtonEnabled(true);
    input_->setStyleSheet(
        "QLineEdit {"
        "  background-color: rgba(5, 8, 20, 235);"
        "  border: 1px solid rgba(76, 108, 255, 170);"
        "  border-radius: 10px;"
        "  padding: 7px 12px;"
        "  color: rgb(240, 248, 255);"
        "  font: 10pt 'Consolas';"
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

    auto* inputGlow = new QGraphicsDropShadowEffect(input_);
    inputGlow->setBlurRadius(22);
    inputGlow->setOffset(0, 0);
    inputGlow->setColor(QColor(120, 150, 255, 105));
    input_->setGraphicsEffect(inputGlow);

    auto* runBtn = new QPushButton("Выполнить", this);
    runBtn->setCursor(Qt::PointingHandCursor);

    auto* btnGlow = new QGraphicsDropShadowEffect(runBtn);
    btnGlow->setBlurRadius(28);
    btnGlow->setOffset(0, 0);
    btnGlow->setColor(QColor(120, 150, 255, 125));
    runBtn->setGraphicsEffect(btnGlow);

    runBtn->setStyleSheet(
       "QPushButton {"
       "  padding: 7px 16px;"
       "  border-radius: 10px;"
       "  border: 1px solid rgba(140, 170, 255, 180);"
       "  color: rgb(240, 248, 255);"
       "  font: 600 10pt 'Segoe UI';"
       "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
       "    stop:0 rgba(26, 37, 80, 230),"
       "    stop:1 rgba(10, 18, 50, 230));"
       "}"
       "QPushButton:hover {"
       "  border: 1px solid rgba(170, 200, 255, 225);"
       "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
       "    stop:0 rgba(34, 48, 100, 240),"
       "    stop:1 rgba(14, 24, 70, 240));"
       "}"
       "QPushButton:pressed {"
       "  padding-top: 8px;"
       "  padding-bottom: 6px;"
       "  background: rgba(12, 18, 50, 240);"
       "}"
       "QPushButton:disabled {"
       "  color: rgba(240, 248, 255, 90);"
       "  border: 1px solid rgba(140, 170, 255, 80);"
       "  background: rgba(20, 28, 60, 160);"
       "}"
   );

    bottom->addWidget(input_, 1);
    bottom->addWidget(runBtn,  0);
    layout->addLayout(bottom);

    connect(input_, &QLineEdit::returnPressed, this, &TerminalDialog::handleReturnPressed);
    connect(runBtn, &QPushButton::clicked, this, &TerminalDialog::handleReturnPressed);

    input_->setFocus();
}

void TerminalDialog::appendText(const QString& text) {
    if (!output_) return;
    output_->appendPlainText(text);
}

static QString viewConfigNameForSave(const QString& saveName) {
    return saveName + ".config.view";
}

static bool writeViewConfigFile(const QString& fileName,
                                const std::unordered_map<std::string, QColor>& cfg) {
    std::ofstream out(fileName.toStdString(), std::ios::out);
    if (!out) return false;

    for (const auto& [path, col] : cfg) {
        out << path << " " << col.red() << " " << col.green() << " " << col.blue() << "\n";
    }
    return true;
}

static std::unordered_map<std::string, QColor> readViewConfigFile(const QString& fileName) {
    std::unordered_map<std::string, QColor> cfg;

    std::ifstream in(fileName.toStdString(), std::ios::in);
    if (!in) return cfg;

    std::string path;
    int r = 0, g = 0, b = 0;
    while (in >> path >> r >> g >> b) {
        cfg[path] = QColor(r, g, b);
    }
    return cfg;
}


/**
 * @brief Обрабатывает выполнение команды
 */
void TerminalDialog::handleReturnPressed() {
    if (!input_) return;

    const QString line = input_->text().trimmed();
    if (line.isEmpty()) {
        input_->clear();
        return;
    }

    appendText(QString("> %1").arg(line));

    std::string res;
    if (terminal_) {
        res = terminal_->send(line.toStdString());

        if (res == "exit") {
            qApp->quit();
            return;
        }

        if (!res.empty())
            appendText(QString::fromStdString(res));
    }

    const QStringList parts = line.split(' ', Qt::SkipEmptyParts);

    if (parts.size() >= 2 && parts[0] == "mkdir") {
        const QString dirName = parts[1];
        if (res.rfind("Directory created", 0) == 0) {
            if (g_graphView)
                g_graphView->addDirectoryNode(dirName);
        }
    }

    if (!parts.isEmpty() && parts[0] == "save") {
        const QString saveName = (parts.size() >= 2) ? parts[1] : "fs.save";
        const QString cfgName  = viewConfigNameForSave(saveName);

        if (g_graphView) {
            const auto cfg = g_graphView->exportViewColorConfig();
            if (!cfg.empty()) {
                writeViewConfigFile(cfgName, cfg);
            }
        }
    }




    if (parts.size() >= 2 && parts[0] == "rm") {
        const QString name = parts[1];
        const bool ok = res.empty()
                        || QString::fromStdString(res).startsWith("File deleted")
                        || QString::fromStdString(res).startsWith("Directory deleted");
        if (ok && g_graphView)
            g_graphView->removeDirectoryByName(name);
    }

    if (parts.size() >= 2 && parts[0] == "cd") {
        const QString target = parts[1];
        if (res.empty() && g_graphView)
            g_graphView->changeDirectoryViewOnly(target);
    }

    if (!parts.isEmpty() && parts[0] == "load") {
        const QString saveName = (parts.size() >= 2) ? parts[1] : "fs.save";
        const QString cfgName  = viewConfigNameForSave(saveName);

        if (g_graphView && this->terminal_) {
            auto fsPtr = this->terminal_->getFileSystem();
            if (fsPtr) {
                g_graphView->rebuildFromFileSystem(*fsPtr);

                const auto cfg = readViewConfigFile(cfgName);
                g_graphView->importViewColorConfig(cfg);
            }
        }
    }



    input_->clear();
}

/**
 * @brief Печатает строку в терминал если он создан
 * @param text Печатаемая строка
 */
void terminalPrint(const QString& text) {
    if (g_terminal) {
        g_terminal->appendText(text);
    }
}
