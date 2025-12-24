#include "replace_plugin.hpp"

#include <memory>
#include <stdexcept>
#include <vector>
#include <iostream>

#include "dataRace.hpp"



std::string ReplaceCommand::execute(const Command& cmd, FileSystem& filesys) {

    if (cmd.args.size() < 3) {
        throw std::invalid_argument(
            "replace <filename> <from> <to> <from2> <to2> ..."
        );
    }

    if ((cmd.args.size() - 1) % 2 != 0) {
        throw std::invalid_argument("must be pairs <from> <to>");
    }

    const std::string& filename = cmd.args[0];

    std::vector<std::string> rules;
    rules.reserve(cmd.args.size() - 1);
    for (size_t i = 1; i < cmd.args.size(); ++i)
        rules.push_back(cmd.args[i]);

    std::string& data = filesys.getDataFromFile(filename);

    const size_t count = replace(data, rules);

    return "replacements = " + std::to_string(count);
}



extern "C" bool get_plugin_data(PluginCommandInfo* out) {
    if (!out) return false;
    out->command_name = "replace";
    out->alias_name   = "ПоменятьЛексемы";
    return true;
}

extern "C" void* create_plugin_command() {
    return static_cast<void*>(new ReplaceCommand());
}

extern "C" void destroy_plugin_command(void* cmd) {
    delete static_cast<ReplaceCommand*>(cmd);
}