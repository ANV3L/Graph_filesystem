#include "accessManager.hpp"

bool check_permissions(const File& file, const User& user, const int i) {
    if (user.UID_ == 0) return true;

    const int permissions = file.getAccessRules();

    if (permissions & (1 << i)) return true;

    if (const uint64_t owner_id = file.getOwner(); user.UID_ == owner_id) {
        if (permissions & (1 << (6 + i))) return true;
    }

    for (auto& g : user.groups) {
        if (const auto ptr = g.lock()) {
            if (ptr->GID_ == file.getGroupOwner()) {
                return static_cast<bool>(permissions & (1 << (3 + i)));
            }
        }
    }

    return false;
}

bool AccessManager::canExecute(const File& file, const User& user) {
    return check_permissions(file, user, 0);
}

bool AccessManager::canWrite(const File &file, const User &user) {
    return check_permissions(file, user, 1);
}


bool AccessManager::canRead(const File& file, const User& user) {
    return check_permissions(file, user, 2);
}

