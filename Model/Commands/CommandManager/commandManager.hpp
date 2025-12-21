#pragma once

#include <memory>
#include <string>
#include <vector>

#include "rootCommand.hpp"
#include "table.hpp"
#include "../PluginHandler/pluginLoader.hpp"

class FileSystem;
class Terminal;

/**
 * @brief Хранит команды и алиасы и выполняет команды над FileSystem
 */
class CommandManager {
public:
    /**
     * @brief Создаёт менеджер и регистрирует команды
     */
    CommandManager();

    /**
     * @brief Возвращает список имён доступных команд и алиасов
     * @return Список строк с именами команд
     */
    std::vector<std::string> getCmds() { return cmds_; }

    /**
     * @brief Добавляет алиас на существующую команду
     * @param alias Имя алиаса
     * @param name_of_new_command Имя существующей команды
     */
    void addCommand(const std::string& alias, const std::string& name_of_new_command);

    /**
     * @brief Удаляет алиас
     * @param alias Имя алиаса
     */
    void deleteCommand(const std::string& alias);

    /**
     * @brief Возвращает строку справки со списком команд
     * @return Строка со списком команд
     */
    [[nodiscard]] std::string help() const;

    /**
     * @brief Парсит строку и выполняет команду над файловой системой
     * @param filesys Файловая система
     * @param line Командная строка
     * @return Результат выполнения команды
     */
    std::string executeCommand(FileSystem& filesys, const std::string& line);

    /**
     * @brief Позволяет добавить новую команду
     */
    void installCommand(const std::string& name, std::shared_ptr<RootCommand> cmd, const std::string* alias);

private:
    std::vector<PluginLoader::LoadedPlugin> installedPlugins_;

    sstd::table<std::string, std::shared_ptr<RootCommand>> commands_;
    sstd::table<std::string, std::shared_ptr<RootCommand>> aliases_;
    std::vector<std::string> cmds_;
};
