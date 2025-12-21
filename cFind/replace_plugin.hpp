#pragma once

#include <string>

#include "../Model/PluginHandler/pluginParser.hpp"
#include "../Model/Commands/interface/rootCommand.hpp"
#include "../Model/Commands/interface/command.hpp"
#include "../Model/Filesystem/filesys.hpp"

class ReplaceCommand final : public RootCommand {
public:
    std::string execute(const Command& cmd, FileSystem& filesys) override;
};

extern "C" {
    bool get_plugin_data(PluginCommandInfo* out);
    void* create_plugin_command();
    void destroy_plugin_command(void* cmd);
}
