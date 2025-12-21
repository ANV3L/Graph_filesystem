#pragma once

#include "files.hpp"
#include "structs.hpp"

/**
 * @brief Проверка прав доступа и управление владельцем файла
 */
struct AccessManager {
    /**
     * @brief Проверяет право на запись
     * @param user Пользователь для проверки
     * @return true если запись разрешена
     */
    static bool canWrite(const File& file, const User& user);

    /**
     * @brief Проверяет право на чтение
     * @param user Пользователь для проверки
     * @return true если чтение разрешено
     */
    static bool canRead(const File& file, const User& user);

    /**
     * @brief Проверяет право на выполнение
     * @param user Пользователь для проверки
     * @return true если выполнение разрешено
     */
    static bool canExecute(const File& file, const User& user);

    /**
     * @brief Назначает владельца файла
     * @param user Новый владелец
     */
    static void makeOwner(File& file, const User& user) { file.setOwner(user.UID_); }

    /**
     * @brief Назначает владельца группы файла
     * @param group Новая группа владелец
     */
    static void makeGroupOwner(File& file, const Group& group) { file.setGroupOwner(group.GID_); }

    /**
     * @brief Возвращает UID владельца файла
     * @return UID владельца
     */
    static uint64_t getOwner(const File& file) { return file.getOwner(); }

    /**
     * @brief Возвращает GID владельца группы файла
     * @return GID владельца группы
     */
    static uint64_t getGroupOwner(const File& file) { return file.getGroupOwner(); }

    /**
     * @brief Возвращает битовую маску прав доступа
     * @return Маска прав доступа
     */
    static int showAccessRules(const File& file) { return file.getAccessRules(); }
};
