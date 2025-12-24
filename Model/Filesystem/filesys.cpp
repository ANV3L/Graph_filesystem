#include "filesys.hpp"

FileSystem::FileSystem(const uint64_t currentUser) : currentUser_(currentUser) {
    users_.insert({0, {0, "administrator"}});
    groups_.insert({0, {0, "admin_group"}});

    root_ = std::make_shared<Directory>(std::shared_ptr<Directory>{}, 0, "home", 0, 0);
    currentDirectory_ = root_;
}

void FileSystem::changeDirectory(const uint64_t id) {
    const auto file = currentDirectory_->getFile(id);

    if (!file) throw std::invalid_argument("File not found");

    const auto dir = std::dynamic_pointer_cast<Directory>(file);
    if (!dir) throw std::invalid_argument("File is not directory");

    currentDirectory_ = dir;
}

uint64_t FileSystem::addUser(const std::string& name) {
    uint64_t newID = nextUserID_++;
    users_.insert({newID, {newID, name}});
    return newID;
}

uint64_t FileSystem::addGroup(const std::string& name) {
    uint64_t newID = nextGroupID_++;
    groups_.insert({newID, {newID, name}});
    return newID;
}

User* FileSystem::getUser(const uint64_t id) {
    const auto it = users_.find(id);
    if (it == users_.end()) return nullptr;
    return &(*it).second;
}

Group* FileSystem::getGroup(const uint64_t id) {
    const auto it = groups_.find(id);
    if (it == groups_.end()) return nullptr;
    return &(*it).second;
}

std::string FileSystem::ListGroups() const {
    std::string result; result.reserve(128);

    for (auto it = groups_.begin(); it != groups_.end(); ++it) {
        result += (*it).second.name_ + " GID = " + std::to_string((*it).first) + '\n';
    }

    return result;
}

std::string FileSystem::ListUsers() const {
    std::string result; result.reserve(128);

    for (auto it = users_.begin(); it != users_.end(); ++it) {
        result += (*it).second.name_ + " UID = " + std::to_string((*it).first) + '\n';
    }

    return result;
}

std::string& FileSystem::getDataFromFile(const std::string& filename){
    auto cur = getCurrentDirectory();
    if (!cur) throw std::runtime_error("no curdir");

    auto f = cur->findByName(filename);
    if (!f) throw std::invalid_argument("file not found");

    auto reg = std::dynamic_pointer_cast<RegularFile>(f);
    if (!reg) throw std::invalid_argument("is not a regular file");

    return reg->getData();
}

void FileSystem::createFile(const std::string& filename){

    auto curd = getCurrentDirectory();

    if (!curd) throw std::runtime_error("Can't get current directory.");

    if (curd->findByName(filename)) throw std::runtime_error("File already exist");

    uint64_t newID = getNextFileID();
    const auto newFile = std::make_shared<RegularFile>(curd, newID, filename, getCurrentUser());

    curd->addFile(newID, newFile);
}