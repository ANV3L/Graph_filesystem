#pragma once

#include <string>
#include <memory>
#include <vector>

struct Group;

/**
 * @brief Представляет собой набор данных о пользователе
 */
struct User {
    uint64_t UID_{};
    std::string name_;
    std::vector<std::weak_ptr<Group>> groups;

    User() = default;
    User(uint64_t const UID, std::string name) : UID_(UID), name_(std::move(name)) {}
};

/**
 * @brief Представляет собой набор данных о группе
 */
struct Group {
    uint64_t GID_{};
    std::string name_;
    std::vector<std::weak_ptr<User>> users;

    Group() = default;
    Group(uint64_t const GID, std::string name) : GID_(GID), name_(std::move(name)) {}
};