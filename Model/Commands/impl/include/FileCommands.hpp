#pragma once

#include "rootCommand.hpp"

/**
 * @brief Базовый класс команд для работы с файлами в текущей директории
 */
class FileCommand : public RootCommand {
protected:
    std::weak_ptr<File> file; ///< Целевой файл для команды
};

/**
 * @brief Команда создания обычного файла
 */
class CreateFileCommand final : public FileCommand {
public:
    std::string execute(const Command& cmd, FileSystem& filesys) override;
};

/**
 * @brief Команда переименования файла
 */
class RenameFileCommand final : public FileCommand {
public:
    std::string execute(const Command& cmd, FileSystem& filesys) override;
};

/**
 * @brief Команда удаления файла или директории по имени
 */
class RemoveFileCommand final : public FileCommand {
public:
    std::string execute(const Command& cmd, FileSystem& filesys) override;
};

/**
 * @brief Команда изменения прав доступа файла
 */
class ChmodFileCommand final : public FileCommand {
public:
    std::string execute(const Command& cmd, FileSystem& filesys) override;
};
