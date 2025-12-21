#pragma once
#include <memory>

#include "table.hpp"
#include "structs.hpp"
#include "files.hpp"

/**
 * @brief Простая модель файловой системы с пользователями, группами и текущей директорией
 */
class FileSystem {
    sstd::table<uint64_t, User> users_;              ///< Таблица пользователей
    sstd::table<uint64_t, Group> groups_;            ///< Таблица групп
    std::shared_ptr<Directory> root_;                ///< Корневая директория
    uint64_t groupAdmin_{0};                         ///< GID админской группы
    uint64_t userAdmin_{0};                          ///< UID администратора

    uint64_t nextFileID_{1};                         ///< Следующий ID файла
    uint64_t nextUserID_{1};                         ///< Следующий UID
    uint64_t nextGroupID_{1};                        ///< Следующий GID

    uint64_t currentUser_ = 0;                       ///< Текущий пользователь
    std::shared_ptr<Directory> currentDirectory_ =
        std::make_shared<Directory>(nullptr, 30000, "home"); ///< Текущая директория

public:
    /**
     * @brief Создаёт файловую систему и инициализирует корень и администратора
     */
    FileSystem(uint64_t currentUser = 0);

    /**
     * @brief Возвращает текущую директорию
     * @return Текущая директория
     */
    [[nodiscard]] std::shared_ptr<Directory> getCurrentDirectory() const { return currentDirectory_; }

    /**
     * @brief Возвращает корневую директорию
     * @return Корень файловой системы
     */
    [[nodiscard]] std::shared_ptr<Directory> getRoot() const { return root_; }

    /**
     * @brief Меняет текущую директорию по ID файла директории
     */
    void changeDirectory(uint64_t id);

    /**
     * @brief Добавляет пользователя
     * @return UID добавленного пользователя
     */
    uint64_t addUser(const std::string& name);

    /**
     * @brief Добавляет группу
     * @return GID добавленной группы
     */
    uint64_t addGroup(const std::string& name);

    /**
     * @brief Удаляет пользователя
     */
    void deleteUser(const uint64_t id) { users_.erase(id); }

    /**
     * @brief Удаляет группу
     */
    void deleteGroup(const uint64_t id) { groups_.erase(id); }

    /**
     * @brief Возвращает пользователя по UID
     * @return Указатель на пользователя или nullptr
     */
    [[nodiscard]] User* getUser(uint64_t id);

    /**
     * @brief Возвращает группу по GID
     * @return Указатель на группу или nullptr
     */
    [[nodiscard]] Group* getGroup(uint64_t id);

    /**
     * @brief Возвращает строку со списком групп
     * @return Список групп
     */
    [[nodiscard]] std::string ListGroups() const;

    /**
     * @brief Возвращает строку со списком пользователей
     * @return Список пользователей
     */
    [[nodiscard]] std::string ListUsers() const;

    /**
     * @brief Возвращает UID текущего пользователя
     * @return UID текущего пользователя
     */
    [[nodiscard]] uint64_t getCurrentUser() const { return currentUser_; }

    /**
     * @brief Устанавливает текущего пользователя
     */
    void setCurrentUser(const uint64_t id) { currentUser_ = id; }

    /**
     * @brief Устанавливает текущую директорию
     */
    void setCurrentDirectory(const std::shared_ptr<Directory>& directory) {
        currentDirectory_ = directory;
    }

    /**
     * @brief Возвращает новый уникальный ID файла и увеличивает счётчик
     * @return Новый ID файла
     */
    uint64_t getNextFileID() { return nextFileID_++; }
};
