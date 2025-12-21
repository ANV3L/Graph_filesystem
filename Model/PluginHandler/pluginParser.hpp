#pragma once

/**
 * @brief структура для передачи информации по плагину
 */
struct PluginCommandInfo {
    const char* command_name; ///< Имя команды в системе
    const char* alias_name = nullptr; ///< Может быть nullptr, если не нужно

};

inline const char* const PluginGetData = "get_plugin_data";
inline const char* const PluginCreateCommand = "create_plugin_command";
inline const char* const PluginDestroyCommand = "destroy_plugin_command";

using PluginGD = bool (*)(PluginCommandInfo* out);
using PluginCC = void* (*)();
using PluginDC = void (*)(void* cmd);