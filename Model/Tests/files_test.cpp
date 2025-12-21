#include <gtest/gtest.h>
#include <memory>
#include <string>

#include "files.hpp"
#include "filesys.hpp"


TEST(FilesCpp, r4)
{
    auto root = std::make_shared<Directory>(std::shared_ptr<Directory>{}, 0, "home", 42, 7);
    auto f    = std::make_shared<RegularFile>(root, 123, "a.txt", 1001, 2002);

    const std::string info = f->getInformation();

    EXPECT_NE(info.find("Create data:"), std::string::npos);
    EXPECT_NE(info.find("Modification data:"), std::string::npos);
    EXPECT_NE(info.find("ID: 123\n"), std::string::npos);
    EXPECT_NE(info.find("Parent directory: home\n"), std::string::npos);
    EXPECT_NE(info.find("Owner: 1001\n"), std::string::npos);
    EXPECT_NE(info.find("Group owner: 2002\n"), std::string::npos);
    EXPECT_NE(info.find("Name: a.txt\n"), std::string::npos);
}

TEST(FilesCpp, r5)
{
    auto root = std::make_shared<Directory>(std::shared_ptr<Directory>{}, 0, "home", 0, 0);

    const std::string info = root->getInformation();

    EXPECT_NE(info.find("Parent directory: null\n"), std::string::npos);
    EXPECT_NE(info.find("Name: home\n"), std::string::npos);
}

TEST(FilesCpp, r2)
{
    auto root = std::make_shared<Directory>(std::shared_ptr<Directory>{}, 0, "home", 0, 0);

    const std::string out = root->ls();
    EXPECT_TRUE(out.empty());
}

TEST(FilesCpp, w2)
{
    auto root = std::make_shared<Directory>(std::shared_ptr<Directory>{}, 0, "home", 0, 0);

    auto f1 = std::make_shared<RegularFile>(root, 1, "a", 0, 0);
    auto f2 = std::make_shared<RegularFile>(root, 2, "b", 0, 0);

    root->addFile(1, f1);
    root->addFile(2, f2);

    const std::string out = root->ls();

    EXPECT_NE(out.find("a "), std::string::npos);
    EXPECT_NE(out.find("b "), std::string::npos);
}


TEST(Files, q1) {
    FileSystem fs;
    fs.setCurrentDirectory(fs.getRoot());

    auto cur = fs.getCurrentDirectory();
    ASSERT_TRUE(cur);

    const uint64_t id = fs.getNextFileID();
    auto rf = std::make_shared<RegularFile>(cur, id, "x", fs.getCurrentUser());
    EXPECT_FALSE(rf->locked());

    rf->makeLocked();
    EXPECT_TRUE(rf->locked());

    rf->unlock();
    EXPECT_FALSE(rf->locked());
}

TEST(Directory, z1) {
    FileSystem fs;
    fs.setCurrentDirectory(fs.getRoot());
    auto cur = fs.getCurrentDirectory();
    ASSERT_TRUE(cur);

    const uint64_t id = fs.getNextFileID();
    auto rf = std::make_shared<RegularFile>(cur, id, "file1", fs.getCurrentUser());
    cur->addFile(id, rf);

    auto byName = cur->findByName("file1");
    ASSERT_TRUE(byName);
    EXPECT_EQ(byName->getID(), id);

    auto byId = cur->getFile(id);
    ASSERT_TRUE(byId);
    EXPECT_EQ(byId->getName(), "file1");

    cur->deleteFile(id);
    EXPECT_FALSE(cur->findByName("file1"));
    EXPECT_FALSE(cur->getFile(id));
}
