#include "RegularFileCommands.hpp"

#include <thread>
#include <vector>
#include <string>

std::string openReadFileCommand::execute(const Command& cmd, FileSystem& filesys) {
    if (cmd.args.size() != 1)
        throw std::invalid_argument("Incorrect arguments count");

    const std::string& filename = cmd.args[0];

    auto curd = filesys.getCurrentDirectory();
    if (!curd)
        throw std::runtime_error("Cant get current directory.");

    auto file = curd->findByName(filename);
    if (!file)
        throw std::invalid_argument("No such file");

    auto reg = std::dynamic_pointer_cast<RegularFile>(file);
    if (!reg)
        throw std::invalid_argument("File is not regular");

    return reg->getData();
}

std::string ListRegularFiles::execute(const Command& cmd, FileSystem& filesys) {
    auto dir = filesys.getCurrentDirectory();
    if (!dir)
        throw std::runtime_error("Can't get current directory.");

    const auto& files = dir->getFiles();

    std::string result;
    bool first = true;

    for (const auto& [id, filePtr] : files) {
        auto reg = std::dynamic_pointer_cast<RegularFile>(filePtr);
        if (!reg)
            continue;

        if (!first)
            result += ' ';
        first = false;

        result += reg->getName();
    }

    return result;
}