#pragma once

#include "rootCommand.hpp"

/**
 * @brief Команда добавления группы в файловую систему
 */
class addGroupCommand final : public RootCommand {
public:
    std::string execute(const Command& cmd, FileSystem& filesys) override;
};

/**
 * @brief Команда добавления пользователя в файловую систему
 */
class addUserCommand final : public RootCommand {
public:
    std::string execute(const Command& cmd, FileSystem& filesys) override;
};

/**
 * @brief Команда удаления группы из файловой системы
 */
class deleteGroupCommand final : public RootCommand {
public:
    std::string execute(const Command& cmd, FileSystem& filesys) override;
};

/**
 * @brief Команда удаления пользователя из файловой системы
 */
class deleteUserCommand final : public RootCommand {
public:
    std::string execute(const Command& cmd, FileSystem& filesys) override;
};

/**
 * @brief Команда сохранения состояния файловой системы
 */
class SaveSystem final : public RootCommand {
public:
    std::string execute(const Command& cmd, FileSystem& filesys) override;
};

/**
 * @brief Команда загрузки состояния файловой системы
 */
class LoadSystem final : public RootCommand {
public:
    std::string execute(const Command& cmd, FileSystem& filesys) override;
};

/**
 * @brief Команда вывода списка групп
 */
class ListGroupsCommand final : public RootCommand {
public:
    std::string execute(const Command& cmd, FileSystem& filesys) override;
};

/**
 * @brief Команда вывода списка пользователей
 */
class ListUsersCommand final : public RootCommand {
public:
    std::string execute(const Command& cmd, FileSystem& filesys) override;
};
