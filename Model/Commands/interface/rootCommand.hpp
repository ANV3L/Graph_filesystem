#pragma once

#include "command.hpp"
#include "../Model/Filesystem/filesys.hpp"
#include <string>

/**
 * @brief Интерфейсная команда для полиморфизма
 */
class  RootCommand {
public:
    virtual std::string execute(const Command& cmd, FileSystem& filesys) = 0;
    virtual ~RootCommand() = default;
};