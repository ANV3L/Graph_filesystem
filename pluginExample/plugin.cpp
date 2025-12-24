#include "plugin.hpp"

std::string class_command_name::execute(const Command& cmd, FileSystem& filesys) {

    std::string filename = cmd.args[0];
    filesys.getDataFromFile(filename) = "OK";
    
    return "OK";
}