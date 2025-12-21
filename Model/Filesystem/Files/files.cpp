#include "files.hpp"
#include <sstream>

File::File(const std::shared_ptr<Directory>& directory, const uint64_t ID, std::string name, const uint64_t owner, const uint64_t groupOwner) : ID_(ID), adress_(directory), name_(std::move(name)), groupOwner_(groupOwner), owner_(owner) {
}

std::string File::getInformation() const {
    std::stringstream result;

    const std::time_t temp = std::chrono::system_clock::to_time_t(createData_);
    result << "Create data: " << std::ctime(&temp);
    const std::time_t temp_modification_time = std::chrono::system_clock::to_time_t(modificationData_);
    result << "Modification data: " << std::ctime(&temp_modification_time);
    result << "ID: " << ID_ << "\n";
    if (const std::shared_ptr<Directory> directory = adress_)
    result << "Parent directory: " << directory->getName() << '\n';
    else
    result << "Parent directory: null\n";
    result << "Owner: " << owner_ << "\n";
    result << "Group owner: " << groupOwner_ << "\n";
    result << "Name: " << name_ << '\n';

    return result.str();
}

std::string Directory::ls() const {
    std::string result;

    for (auto it = files_.begin(); it != files_.end(); ++it) {
        result += (*it).second->getName() + ' ';
    }

    return result;
}


std::shared_ptr<File> Directory::getFile(const uint64_t id) const {
    auto it = files_.find(id);
    if (it == files_.end()) return nullptr;

    return (*it).second;
}

std::shared_ptr<File> Directory::findByName(const std::string& name) const {
    for (auto it = files_.begin(); it != files_.end(); ++it) {
        if ((*it).second->getName() == name) return (*it).second;
    }
    return nullptr;
}