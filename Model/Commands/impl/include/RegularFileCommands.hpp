#pragma once

#include "FileCommands.hpp"

/**
 * @brief Базовый класс команд для работы с обычными файлами
 */
class RegularFileCommand : public FileCommand {
};

/**
 * @brief Команда чтения содержимого обычного файла
 */
class openReadFileCommand final : public RegularFileCommand {
public:
    std::string execute(const Command& cmd, FileSystem& filesys) override;
};

/**
 * @brief Команда вывода списка обычных файлов в текущей директории
 */
class ListRegularFiles final : public RegularFileCommand {
public:
    std::string execute(const Command& cmd, FileSystem& filesys) override;
};