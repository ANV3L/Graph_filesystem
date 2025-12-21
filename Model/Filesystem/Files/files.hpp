#pragma once

#include <chrono>
#include <memory>
#include <string>

#include "table.hpp"

class Directory;

/**
 * @brief Базовый класс файловой сущности с владельцами и правами доступа
 */
class File {
protected:
    int access_rules_ = 0b110100100; ///< Битовая маска прав доступа
    const std::chrono::system_clock::time_point createData_{std::chrono::system_clock::now()}; ///< Время создания
    std::chrono::system_clock::time_point modificationData_{std::chrono::system_clock::now()}; ///< Время последнего изменения
    uint64_t ID_{};                  ///< Идентификатор файла
    std::shared_ptr<Directory> adress_; ///< Родительская директория
    std::string name_{};             ///< Имя файла
    uint64_t groupOwner_{};          ///< GID владельца группы
    uint64_t owner_{};               ///< UID владельца

public:
    File() = default;
    virtual ~File() = default;

    /**
     * @brief Создаёт файловую сущность с заданными атрибутами
     */
    File(const std::shared_ptr<Directory>& directory, uint64_t ID, std::string name, uint64_t owner = 0, uint64_t groupOwner = 0);

    /**
     * @brief Возвращает строку с информацией о файле
     * @return Информация о файле
     */
    [[nodiscard]] std::string getInformation() const;

    /**
     * @brief Возвращает имя файла
     * @return Имя файла
     */
    [[nodiscard]] const std::string& getName() const { return name_; }

    /**
     * @brief Возвращает идентификатор файла
     * @return ID файла
     */
    [[nodiscard]] uint64_t getID() const { return ID_; }

    /**
     * @brief Возвращает UID владельца
     * @return UID владельца
     */
    [[nodiscard]] uint64_t getOwner() const { return owner_; }

    /**
     * @brief Возвращает GID владельца группы
     * @return GID владельца группы
     */
    [[nodiscard]] uint64_t getGroupOwner() const { return groupOwner_; }

    /**
     * @brief Возвращает битовую маску прав доступа
     * @return Маска прав доступа
     */
    [[nodiscard]] int getAccessRules() const { return access_rules_; }

    /**
     * @brief Возвращает родительскую директорию
     * @return Родительская директория или nullptr
     */
    [[nodiscard]] std::shared_ptr<Directory> getParent() const { return adress_; }

    /**
     * @brief Устанавливает маску прав доступа
     */
    void setAccessRules(const int accessRules) { access_rules_ = accessRules; }

    /**
     * @brief Переименовывает файл
     */
    void rename(const std::string& name) { name_ = name; }

    /**
     * @brief Устанавливает UID владельца
     */
    void setOwner(const uint64_t owner) { owner_ = owner; }

    /**
     * @brief Устанавливает GID владельца группы
     */
    void setGroupOwner(const uint64_t owner) { groupOwner_ = owner; }
};

/**
 * @brief Директория с таблицей дочерних файлов
 */
class Directory final : public File {
    sstd::table<uint64_t, std::shared_ptr<File>> files_; ///< Содержимое директории

public:
    Directory() = delete;

    /**
     * @brief Создаёт директорию и задаёт права доступа директории
     */
    Directory(const std::shared_ptr<Directory>& directory, const uint64_t ID, const std::string& name, const uint64_t owner = 0, const uint64_t groupOwner = 0)
        : File(directory, ID, name, owner, groupOwner) { access_rules_ = 0b111101101; }

    /**
     * @brief Возвращает список имён файлов в директории
     * @return Список имён
     */
    [[nodiscard]] std::string ls() const;

    /**
     * @brief Возвращает таблицу файлов директории
     * @return Таблица файлов
     */
    [[nodiscard]] const sstd::table<uint64_t, std::shared_ptr<File>>& getFiles() const { return files_; }

    /**
     * @brief Ищет файл по имени
     * @return Указатель на файл или nullptr
     */
    [[nodiscard]] std::shared_ptr<File> findByName(const std::string& name) const;

    /**
     * @brief Добавляет файл в директорию
     */
    void addFile(const uint64_t id, const std::shared_ptr<File>& file) { files_.insert({id, file}); }

    /**
     * @brief Удаляет файл из директории по ID
     */
    void deleteFile(const uint64_t id) { files_.erase(id); }

    /**
     * @brief Возвращает файл по ID
     * @return Указатель на файл или nullptr
     */
    [[nodiscard]] std::shared_ptr<File> getFile(uint64_t id) const;
};

/**
 * @brief Обычный файл с содержимым и флагом блокировки
 */
class RegularFile final : public File {
    bool lock_ = false;      ///< Флаг блокировки
    std::string data_;       ///< Содержимое файла

public:
    RegularFile() = delete;

    /**
     * @brief Создаёт обычный файл
     */
    RegularFile(const std::shared_ptr<Directory>& directory, const uint64_t ID, const std::string& name, const uint64_t owner = 0, uint64_t groupOwner = 0)
        : File(directory, ID, name, owner, groupOwner) {}

    /**
     * @brief Возвращает состояние блокировки
     * @return true если файл заблокирован
     */
    [[nodiscard]] bool locked() const { return lock_; }

    /**
     * @brief Блокирует файл
     */
    void makeLocked() noexcept { lock_ = true; }

    /**
     * @brief Снимает блокировку файла
     */
    void unlock() noexcept { lock_ = false; }

    /**
     * @brief Возвращает содержимое файла
     * @return Содержимое файла
     */
    std::string& getData() { return data_; }

    /**
     * @brief Устанавливает содержимое файла
     */
    void setData(const std::string& data) { data_ = data; }
};
