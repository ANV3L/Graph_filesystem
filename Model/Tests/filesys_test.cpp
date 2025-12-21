#include <gtest/gtest.h>
#include <memory>

#include "filesys.hpp"
#include "files.hpp"

TEST(FileSystem, d1) {
    FileSystem fs;
    ASSERT_TRUE(fs.getRoot());
    ASSERT_TRUE(fs.getCurrentDirectory());
    EXPECT_EQ(fs.getRoot()->getName(), "home");
    EXPECT_EQ(fs.getCurrentDirectory()->getName(), "home");
}

TEST(FileSystem, d2) {
    FileSystem fs;
    EXPECT_THROW(fs.changeDirectory(999999), std::invalid_argument);
}

TEST(FileSystem, d3) {
    FileSystem fs;
    fs.setCurrentDirectory(fs.getRoot());
    auto cur = fs.getCurrentDirectory();
    ASSERT_TRUE(cur);

    const uint64_t id = fs.getNextFileID();
    cur->addFile(id, std::make_shared<RegularFile>(cur, id, "a", fs.getCurrentUser()));

    EXPECT_THROW(fs.changeDirectory(id), std::invalid_argument);
}

TEST(FileSystem, d4) {
    FileSystem fs;

    const uint64_t u1 = fs.addUser("u1");
    const uint64_t u2 = fs.addUser("u2");
    EXPECT_NE(u1, u2);
    EXPECT_NE(fs.getUser(u1), nullptr);
    EXPECT_NE(fs.getUser(u2), nullptr);

    const uint64_t g1 = fs.addGroup("g1");
    const uint64_t g2 = fs.addGroup("g2");
    EXPECT_NE(g1, g2);
    EXPECT_NE(fs.getGroup(g1), nullptr);
    EXPECT_NE(fs.getGroup(g2), nullptr);

    const auto users = fs.ListUsers();
    const auto groups = fs.ListGroups();
    EXPECT_NE(users.find("u1"), std::string::npos);
    EXPECT_NE(groups.find("g1"), std::string::npos);
}
