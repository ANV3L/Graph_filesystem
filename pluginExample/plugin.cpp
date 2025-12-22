#include "plugin.hpp"

std::string testCommand::execute(const Command& cmd, FileSystem& filesys) {

    return "OK";

}



extern "C" bool get_plugin_data(PluginCommandInfo* out) {
    if (!out) return false;
    out->command_name = "ok";
    out->alias_name   = "OK";
    return true;
}

extern "C" void* create_plugin_command() {
    return static_cast<void*>(new testCommand());
}

extern "C" void destroy_plugin_command(void* cmd) {
    delete static_cast<testCommand*>(cmd);
}