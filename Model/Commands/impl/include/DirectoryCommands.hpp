#pragma once

#include "FileCommands.hpp"

/**
 * @brief Базовый класс команд для работы с директориями
 */
class DirectoryCommand : public FileCommand {
};

/**
 * @brief Команда вывода содержимого текущей директории
 */
class ListDirectoryCommand final : public DirectoryCommand {
public:
    std::string execute(const Command& cmd, FileSystem& filesys) override;
};

/**
 * @brief Команда вывода текущего пути
 */
class PrintWorkingDirectory final : public RootCommand {
public:
    std::string execute(const Command& cmd, FileSystem& filesys) override;
};

/**
 * @brief Команда создания директории
 */
class CreateDirectoryCommand final : public RootCommand {
public:
    std::string execute(const Command& cmd, FileSystem& filesys) override;
};

/**
 * @brief Команда смены текущей директории
 */
class ChangeDirectory final : public RootCommand {
public:
    std::string execute(const Command& cmd, FileSystem& filesys) override;
};
