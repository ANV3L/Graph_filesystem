#pragma once

#include "filesys.hpp"
#include "commandManager.hpp"
#include "accessManager.hpp"
#include <iostream>

class Terminal {
    std::shared_ptr<FileSystem> fileSystem_;
    bool initialized_{false};
    CommandManager commandManager_{};

public:
    Terminal() = default;
    ~Terminal() = default;
    Terminal(const Terminal &) = delete;
    Terminal &operator=(const Terminal &) = delete;
    Terminal(Terminal &&) = delete;
    Terminal &operator=(Terminal &&) = delete;

    std::vector<std::string> getCmds() {
        return commandManager_.getCmds();
    };

    void run(std::istream& istream = std::cin, std::ostream& ostream = std::cout) ;
    void initialize();

    std::string send(const std::string& line);
    [[nodiscard]] std::string& readFileToString(const std::string& filename) const;
    [[nodiscard]] std::shared_ptr<FileSystem> getFileSystem() const { return fileSystem_; }
};