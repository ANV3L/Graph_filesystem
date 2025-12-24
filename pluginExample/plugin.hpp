#pragma once

#include "../include/filesystem_for_plugin.hpp"

#include <string>

// DEFINE AREA START ---------------------------

#define c_class_command_name TestCommand // Напишите вместо "_" то, как вы бы хотели, чтобы назывался класс вашей команды

#define c_command_name "OK" // Напишите вместо "Your command_name" как бы вы хотели, чтобы команду звали

#define c_alias_name "ok" // Напишите вместо "Your alias" как бы вы хотели, чтобы звали команду в карусели команд

// DEFINE AREA END -----------------------------




// Корреляция с файловой системой, тут ничего трогать не нужно

class class_command_name final : public RootCommand {
public:
    std::string execute(const Command& cmd, FileSystem& filesys) override;
};

extern "C" {
    bool get_plugin_data(PluginCommandInfo* out);
    void* create_plugin_command();
    void destroy_plugin_command(void* cmd);
}

extern "C" bool get_plugin_data(PluginCommandInfo* out) {
    if (!out) return false;
    out->command_name = c_command_name;
    out->alias_name   = c_alias_name;
    return true;
}

extern "C" void* create_plugin_command() {
    return static_cast<void*>(new class_command_name());
}

extern "C" void destroy_plugin_command(void* cmd) {
    delete static_cast<class_command_name*>(cmd);
}