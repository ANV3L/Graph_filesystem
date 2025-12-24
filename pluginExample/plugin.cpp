#include "plugin.hpp"

std::string class_command_name::execute(const Command& cmd, FileSystem& filesys) {

    if (cmd.args.empty()) throw std::invalid_argument("No filename");
    
    std::string filename = cmd.args[0];
    filesys.getDataFromFile(filename) = "OK";

    filesys.createFile("SWAGA");
    
    return "OK";
}