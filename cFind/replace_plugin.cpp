#include "replace_plugin.hpp"

#include <memory>
#include <stdexcept>
#include <vector>

#include "../Model/Filesystem/Files/files.hpp"
#include "dataRace.hpp"
#include <iostream>



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

    auto cur = filesys.getCurrentDirectory();
    if (!cur) throw std::runtime_error("no curdir");

    auto f = cur->findByName(filename);
    if (!f) throw std::invalid_argument("file not found");

    auto reg = std::dynamic_pointer_cast<RegularFile>(f);
    if (!reg) throw std::invalid_argument("is not a regular file");

    std::vector<std::string> rules;
    rules.reserve(cmd.args.size() - 1);
    for (size_t i = 1; i < cmd.args.size(); ++i)
        rules.push_back(cmd.args[i]);

    std::string data = reg->getData();
    const size_t count = replace(data, rules);
    reg->setData(data);

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