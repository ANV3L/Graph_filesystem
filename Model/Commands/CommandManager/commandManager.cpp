#include <fstream>

#include "commandManager.hpp"

#include "commands.hpp"
#include "DirectoryCommands.hpp"
#include "FileCommands.hpp"
#include "RegularFileCommands.hpp"
#include "command.hpp"

CommandManager::CommandManager() : cmds_{"Открыть"} {
    commands_.insert({"ls", std::make_shared<ListDirectoryCommand>()});
    commands_.insert({"touch", std::make_shared<CreateFileCommand>()});
    commands_.insert({"mkdir", std::make_shared<CreateDirectoryCommand>()});
    commands_.insert({"rename", std::make_shared<RenameFileCommand>()});
    commands_.insert({"rm", std::make_shared<RemoveFileCommand>()});
    commands_.insert({"chmod", std::make_shared<ChmodFileCommand>()});
    commands_.insert({"open", std::make_shared<openReadFileCommand>()});
    commands_.insert({"addgroup", std::make_shared<addGroupCommand>()});
    commands_.insert({"adduser", std::make_shared<addUserCommand>()});
    commands_.insert({"lsgroups", std::make_shared<ListGroupsCommand>()});
    commands_.insert({"lsusers", std::make_shared<ListUsersCommand>()});
    commands_.insert({"save", std::make_shared<SaveSystem>()});
    commands_.insert({"load", std::make_shared<LoadSystem>()});
    commands_.insert({"pwd", std::make_shared<PrintWorkingDirectory>()});
    commands_.insert({"lsRegular", std::make_shared<ListRegularFiles>()});
    commands_.insert({"cd", std::make_shared<ChangeDirectory>()});

    aliases_.insert({"Вывести", std::make_shared<openReadFileCommand>()});
    aliases_.insert({"Переименовать", std::make_shared<RenameFileCommand>()});

    for (const auto& c : aliases_) {
        cmds_.push_back(c.first);
    }
}

#include <iostream>
std::string CommandManager::executeCommand(FileSystem& filesys, const std::string& line) {
    const Command cmd{line};

    if (cmd.name == "install") {
        if (cmd.args.size() != 1)
            throw std::invalid_argument("No install path");

        PluginLoader loader;
        auto plugin = loader.load(cmd.args[0]);

        std::string aliasStr;
        const std::string* aliasPtr = nullptr;
        if (plugin.info.alias_name && plugin.info.alias_name[0] != '\0') {
            aliasStr = plugin.info.alias_name;
            aliasPtr = &aliasStr;
        }

        installCommand(plugin.info.command_name, plugin.command, aliasPtr);

        installedPlugins_.push_back(std::move(plugin));

        return std::string("Installed: ") + installedPlugins_.back().info.command_name;
    }

    if (cmd.name.empty()) return {};


    if (cmd.name == "alias") {
        aliases_.insert({cmd.args[0], commands_.find(cmd.args[1])->second});
        return "";
    }

    auto it = commands_.find(cmd.name);

    if (it == commands_.end()) {
        auto i = aliases_.find(cmd.name);
        if (i == aliases_.end())
            throw std::invalid_argument("Command not found: " + cmd.name);
        return (*i).second->execute(cmd, filesys);
    }

    return (*it).second->execute(cmd, filesys);
}

void CommandManager::addCommand(const std::string& alias, const std::string& name_of_new_command) {
    auto iter = commands_.find(name_of_new_command);
    if (iter == commands_.end()) throw std::invalid_argument("Command not found: " + name_of_new_command);
    std::shared_ptr<RootCommand> ptr = iter->second;

    aliases_.insert_or_assign({alias, ptr});
}

void CommandManager::deleteCommand(const std::string& alias) {
    aliases_.erase(alias);
}

std::string CommandManager::help() const {
    std::string result;

    for (const auto& [name, _] : commands_)
        result += name + '\n';

    for (const auto& [name, _] : aliases_)
        result += name + '\n';

    return result;
}

void CommandManager::installCommand(const std::string& name,
                                   std::shared_ptr<RootCommand> cmd,
                                   const std::string* alias)
{
    if (!cmd) throw std::invalid_argument("installCommand: null cmd");

    commands_.insert_or_assign({name, cmd});

    if (alias && !alias->empty()) {
        aliases_.insert_or_assign({*alias, cmd});
        cmds_.push_back(*alias);
    }
}