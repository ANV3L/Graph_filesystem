#pragma once

#include "terminal.hpp"
#include "graph/GraphView.hpp"

/**
 * @brief Обёртка над Terminal и GraphView для запуска графического файлового интерфейса
 */
class GraphFileSystem {
public:
    /**
     * @brief Создаёт объект и инициализирует терминал
     */
    GraphFileSystem() {
        terminal_.initialize();
    }

    /**
     * @brief Запускает приложение
     * @param argc Количество аргументов командной строки
     * @param argv Массив аргументов командной строки
     */
    void run(int argc, char* argv[]);

private:
    GraphView* graphView_ = nullptr; ///< Указатель на активный GraphView
    Terminal terminal_;              ///< Терминал приложения
};
