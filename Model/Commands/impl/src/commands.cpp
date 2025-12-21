#include <fstream>
#include <functional>
#include <stdexcept>

#include "commands.hpp"
#include <sstream>
#include <string>

#include "commandManager.hpp"
#include "files.hpp"

std::string addGroupCommand::execute(const Command & cmd, FileSystem &filesys) {
    if (cmd.args.empty()) throw std::invalid_argument("No group name");

    const uint64_t gid = filesys.addGroup(cmd.args[0]);

    return "Group added, GID = " + std::to_string(gid);
}

std::string addUserCommand::execute(const Command & cmd, FileSystem &filesys) {
    if (cmd.args.empty()) throw std::invalid_argument("No user name");

    const uint64_t uid = filesys.addUser(cmd.args[0]);

    return "User added, UID = " + std::to_string(uid);
}

std::string deleteGroupCommand::execute(const Command & cmd, FileSystem &filesys) {
    if (cmd.args.empty()) throw std::invalid_argument("No group name");

    uint64_t uid;
    try {
        uid = std::stoull(cmd.args[0]);
    } catch (...) {
        throw std::invalid_argument("Incorrect format for gid");
    }

    if (!filesys.getGroup(uid)) throw std::invalid_argument("No group in groups table");

    filesys.deleteGroup(uid);

    return "Group deleted, GID = " + std::to_string(uid);
}

std::string deleteUserCommand::execute(const Command & cmd, FileSystem &filesys) {
    if (cmd.args.empty()) throw std::invalid_argument("No user name");

    uint64_t uid;
    try {
        uid = std::stoull(cmd.args[0]);
    } catch (...) {
        throw std::invalid_argument("Incorrect format for uid");
    }

    if (!filesys.getUser(uid)) throw std::invalid_argument("No user in users table");

    filesys.deleteUser(uid);

    return "User deleted, UID = " + std::to_string(uid);
}

std::string ListGroupsCommand::execute(const Command & cmd, FileSystem & filesys) {
    return filesys.ListGroups();
}

std::string ListUsersCommand::execute(const Command & cmd, FileSystem & filesys) {
    return filesys.ListUsers();
}

std::string SaveSystem::execute(const Command& cmd, FileSystem& filesys) {
    const std::string outName = cmd.args.empty() ? "fs.save" : cmd.args[0];

    std::ofstream out(outName, std::ios::out);
    if (!out)
        throw std::runtime_error("Cant open save file");

    auto root = filesys.getRoot();
    if (!root)
        throw std::runtime_error("No root directory");

    auto writeDataBlock = [&](const std::string& data) {
        out << "<start>\n";
        out << data;
        if (data.empty() || data.back() != '\n')
            out << "\n";
        out << "<end>\n";
    };

    std::function<void(const std::shared_ptr<Directory>&)> dumpDir;
    dumpDir = [&](const std::shared_ptr<Directory>& dir) {
        const auto files = dir->getFiles();

        for (auto it = files.begin(); it != files.end(); ++it) {
            const auto& f = it->second;
            if (!f) continue;

            auto subdir = std::dynamic_pointer_cast<Directory>(f);
            if (!subdir) continue;

            out << "mkdir " << subdir->getName() << "\n";
            out << "cd " << subdir->getName() << "\n";
            dumpDir(subdir);
            out << "cd ..\n";
        }

        for (auto it = files.begin(); it != files.end(); ++it) {
            const auto& f = it->second;
            if (!f) continue;

            auto reg = std::dynamic_pointer_cast<RegularFile>(f);
            if (!reg) continue;

            out << "touch " << reg->getName() << "\n";
            writeDataBlock(reg->getData());
        }
    };

    dumpDir(root);

    out.flush();
    if (!out)
        throw std::runtime_error("Save write error");

    return "Saved to " + outName;
}

std::string LoadSystem::execute(const Command& cmd, FileSystem& filesys) {
    const std::string inName = cmd.args.empty() ? "fs.save" : cmd.args[0];

    std::ifstream in(inName);
    if (!in)
        throw std::runtime_error("Cant open load file");

    filesys = FileSystem(0);

    CommandManager mgr;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty())
            continue;
        if (!line.empty() && line[0] == '#')
            continue;

        if (line.rfind("touch ", 0) == 0) {
            mgr.executeCommand(filesys, line);

            std::istringstream iss(line);
            std::string cmdName, filename;
            iss >> cmdName >> filename;
            if (filename.empty())
                throw std::runtime_error("touch without filename in save script");

            std::string marker;
            if (!std::getline(in, marker) || marker != "<start>")
                throw std::runtime_error("Expected <start> after touch " + filename);

            std::string data;
            std::string chunk;
            bool endFound = false;

            while (std::getline(in, chunk)) {
                if (chunk == "<end>") {
                    endFound = true;
                    break;
                }
                data += chunk;
                data += '\n';
            }

            if (!endFound)
                throw std::runtime_error("Unexpected EOF: no <end> for file " + filename);

            auto curd = filesys.getCurrentDirectory();
            if (!curd)
                throw std::runtime_error("Cant get current directory");

            auto f = curd->findByName(filename);
            if (!f)
                throw std::runtime_error("File not found after touch: " + filename);

            auto reg = std::dynamic_pointer_cast<RegularFile>(f);
            if (!reg)
                throw std::runtime_error("Touched object is not RegularFile: " + filename);

            reg->getData() = data;

            continue;
        }

        mgr.executeCommand(filesys, line);
    }

    return "Loaded from " + inName;
}