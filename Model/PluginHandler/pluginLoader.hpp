#pragma once

#include <string>
#include <memory>

#include "rootCommand.hpp"
#include "pluginParser.hpp"

/**
* @brief Загрузчик динамических плагинов команд
*/

struct PluginLoader {
    struct LoadedPlugin {
      PluginCommandInfo info{};
        std::shared_ptr<RootCommand> command;
        std::shared_ptr<void> lib;
    };

    LoadedPlugin load(const std::string& path);
};
