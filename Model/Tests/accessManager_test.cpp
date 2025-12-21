#include <gtest/gtest.h>
#include <memory>
#include <string>

#include "accessManager.hpp"
#include "filesys.hpp"
#include "files.hpp"
#include "structs.hpp"

static std::shared_ptr<RegularFile> makeFile(FileSystem& fs,
                                             const std::string& name,
                                             uint64_t ownerUid,
                                             uint64_t groupGid,
                                             int accessOctal)
{
    auto root = fs.getRoot();
    EXPECT_TRUE(root);
    fs.setCurrentDirectory(root);

    const uint64_t id = fs.getNextFileID();
    auto f = std::make_shared<RegularFile>(root, id, name, ownerUid, groupGid);
    root->addFile(id, f);

    f->setAccessRules(accessOctal);
    return f;
}

TEST(Permissions, a1)
{
    FileSystem fs;

    const uint64_t gid = fs.addGroup("grp1");
    const uint64_t uid = fs.addUser("user1");

    EXPECT_NE(fs.ListGroups().find("grp1"), std::string::npos);
    EXPECT_NE(fs.ListUsers().find("user1"), std::string::npos);

    auto grpPtr = std::make_shared<Group>(gid, "grp1");
    User u(uid, "user1");
    u.groups.push_back(grpPtr);

    auto file = makeFile(fs, "a.txt", 9999, gid, 0040);

    ASSERT_TRUE(file);

    EXPECT_TRUE(AccessManager::canRead(*file, u));
    EXPECT_FALSE(AccessManager::canWrite(*file, u));
    EXPECT_FALSE(AccessManager::canExecute(*file, u));

    User other(uid + 100, "other");
    EXPECT_FALSE(AccessManager::canRead(*file, other));

    fs.deleteUser(uid);
    fs.deleteGroup(gid);

    EXPECT_EQ(fs.ListUsers().find("user1"), std::string::npos);
    EXPECT_EQ(fs.ListGroups().find("grp1"), std::string::npos);
}


static std::shared_ptr<RegularFile> addRegular(FileSystem& fs, const std::string& name) {
    auto cur = fs.getCurrentDirectory();
    if (!cur) throw std::runtime_error("no curdir");

    const uint64_t id = fs.getNextFileID();
    auto f = std::make_shared<RegularFile>(cur, id, name, fs.getCurrentUser());
    cur->addFile(id, f);
    return f;
}

TEST(AccessManager, a2) {
    FileSystem fs;
    auto root = fs.getRoot();
    ASSERT_TRUE(root);
    fs.setCurrentDirectory(root);

    auto f = addRegular(fs, "a.txt");

    User rootUser{0, "administrator"};
    EXPECT_TRUE(AccessManager::canRead(*f, rootUser));
    EXPECT_TRUE(AccessManager::canWrite(*f, rootUser));
    EXPECT_TRUE(AccessManager::canExecute(*f, rootUser));
}

TEST(AccessManager, a3) {
    FileSystem fs;
    fs.setCurrentDirectory(fs.getRoot());

    auto f = addRegular(fs, "a.txt");

    f->setAccessRules((1 << 0) | (1 << 1) | (1 << 2));

    User u{10, "u"};
    EXPECT_TRUE(AccessManager::canExecute(*f, u));
    EXPECT_TRUE(AccessManager::canWrite(*f, u));
    EXPECT_TRUE(AccessManager::canRead(*f, u));
}

TEST(AccessManager, a4) {
    FileSystem fs;
    fs.setCurrentDirectory(fs.getRoot());

    auto f = addRegular(fs, "a.txt");

    User owner{42, "owner"};
    AccessManager::makeOwner(*f, owner);

    f->setAccessRules(1 << 8);

    EXPECT_TRUE(AccessManager::canRead(*f, owner));
    EXPECT_FALSE(AccessManager::canWrite(*f, owner));
    EXPECT_FALSE(AccessManager::canExecute(*f, owner));

    User other{43, "other"};
    EXPECT_FALSE(AccessManager::canRead(*f, other));
}
