#include "FileCommands.hpp"
#include "files.hpp"

std::string CreateFileCommand::execute(const Command& cmd, FileSystem& filesys) {
    if (cmd.args.empty()) throw std::invalid_argument("No file name");

    auto curd = filesys.getCurrentDirectory();

    if (!curd) throw std::runtime_error("Can't get current directory.");

    if (curd->findByName(cmd.args[0])) throw std::runtime_error("File already exist");

    uint64_t newID = filesys.getNextFileID();
    const auto newFile = std::make_shared<RegularFile>(curd, newID, cmd.args[0], filesys.getCurrentUser());

    curd->addFile(newID, newFile);

    return "File created, uid = " + std::to_string(newID);
}


std::string RenameFileCommand::execute(const Command& cmd, FileSystem& filesys) {
    if (cmd.args.size() < 2)
        throw std::invalid_argument("No <old name> <new name>");

    const std::string& oldName = cmd.args[0];
    const std::string& newName = cmd.args[1];

    if (newName.empty())
        throw std::invalid_argument("Empty new name");

    auto curd = filesys.getCurrentDirectory();
    if (!curd)
        throw std::runtime_error("Cant get current directory.");

    if (oldName == ".") {
        auto parent = curd->getParent();
        if (parent) {
            if (parent->findByName(newName))
                throw std::runtime_error("File with new name already exist");
        }
        curd->rename(newName);
        return "";
    }

    if (curd->findByName(newName))
        throw std::runtime_error("File with new name already exist");

    auto file = curd->findByName(oldName);
    if (!file)
        throw std::invalid_argument("Cant find file");

    file->rename(newName);

    return "";
}



std::string RemoveFileCommand::execute(const Command& cmd, FileSystem& filesys) {
    if (cmd.args.empty()) throw std::invalid_argument("No file name");

    const auto curd = filesys.getCurrentDirectory();

    if (!curd) throw std::runtime_error("Can't get current directory.");

    const auto file = curd->findByName(cmd.args[0]);
    if (!file) throw std::invalid_argument("Can't find file");

    curd->deleteFile(file->getID());

    return "File deleted";
}

std::string ChmodFileCommand::execute(const Command& cmd, FileSystem& filesys) {
    if (cmd.args.size() < 2) throw std::invalid_argument("Incorrect format (<filename> <permissions>)");

    const std::string& filename = cmd.args[0];
    const std::string& accessStr = cmd.args[1];

    auto curd = filesys.getCurrentDirectory();
    if (!curd) throw std::runtime_error("Can't get current directory.");

    auto file = curd->findByName(filename);
    if (!file) throw std::invalid_argument("No such file in directory");
    int access;
    try {
        access = std::stoi(accessStr, nullptr, 8);
    } catch (...) {
        throw std::invalid_argument("Incorrect format");
    }

    file->setAccessRules(access);
    return "";
}

