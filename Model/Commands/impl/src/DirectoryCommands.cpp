#include "DirectoryCommands.hpp"

std::string ListDirectoryCommand::execute(const Command& cmd, FileSystem& filesys) {
    const auto curd = filesys.getCurrentDirectory();

    if (!curd) throw std::runtime_error("Can't get current directory.");

    return curd->ls();
}

std::string PrintWorkingDirectory::execute(const Command &cmd, FileSystem &filesys) {
    std::shared_ptr<Directory> dir = filesys.getCurrentDirectory();
    std::vector<std::string> res;
    res.push_back(dir->getName());
    while ((dir = dir->getParent()) != nullptr) {
        res.push_back(dir->getName());
    }
    std::string ans;

    for (size_t i = res.size() - 1; i; --i) {
        ans += res[i] + "\\";
    }
    ans += res[0];
    return ans;
}

std::string CreateDirectoryCommand::execute(const Command &cmd, FileSystem &filesys) {
    if (cmd.args.empty()) throw std::invalid_argument("No file name");

    auto curd = filesys.getCurrentDirectory();

    if (!curd) throw std::runtime_error("Can't get current directory.");

    if (curd->findByName(cmd.args[0])) throw std::runtime_error("File already exist");

    uint64_t newID = filesys.getNextFileID();
    const auto newFile = std::make_shared<Directory>(curd, newID, cmd.args[0], filesys.getCurrentUser());

    curd->addFile(newID, newFile);

    return "Directory created, uid = " + std::to_string(newID);
}

std::string ChangeDirectory::execute(const Command &cmd, FileSystem &filesys) {
    if (cmd.args.empty())
        throw std::invalid_argument("No directory name");

    const std::string &arg = cmd.args[0];

    auto curd = filesys.getCurrentDirectory();
    if (!curd)
        throw std::runtime_error("Cant get current directory.");

    if (arg == "..") {
        auto parent = curd->getParent();
        if (!parent) {
            return "";
        }
        filesys.setCurrentDirectory(parent);
        return "";
    }

    auto file = curd->findByName(arg);
    if (!file)
        throw std::invalid_argument("No such file or directory");

    auto dir = std::dynamic_pointer_cast<Directory>(file);
    if (!dir)
        throw std::invalid_argument("File is not directory");

    filesys.setCurrentDirectory(dir);
    return "";
}
