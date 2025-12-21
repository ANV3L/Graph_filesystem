#pragma once

#include <string>
#include <vector>
#include <sstream>

/**
 * @brief Структура команды (Поддерживает преобразование из std::string в Command)
 */

struct Command {
    std::string name{};
    std::vector<std::string> args{};
    Command() = delete;

    explicit Command(const std::string& line) {
        if (line.empty()) return;

        std::istringstream stream(line);
        stream >> name;

        std::string arg;
        while (stream >> arg) args.emplace_back(arg);
    }
};

