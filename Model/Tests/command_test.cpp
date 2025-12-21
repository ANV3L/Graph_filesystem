#include <gtest/gtest.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

#include "command.hpp"
#include "commands.hpp"
#include "files.hpp"
#include "filesys.hpp"

TEST(CommandsCpp, a1)
{
    FileSystem fs;
    deleteGroupCommand cmd;
    EXPECT_THROW(cmd.execute(Command{"deletegroup"}, fs), std::invalid_argument);
}

TEST(CommandsCpp, a2)
{
    FileSystem fs;
    deleteUserCommand cmd;
    EXPECT_THROW(cmd.execute(Command{"deleteuser"}, fs), std::invalid_argument);
}


static std::string makeTempPath(const std::string& baseName)
{
    namespace fs = std::filesystem;

    fs::path dir;
    try {
        dir = fs::temp_directory_path();
    } catch (...) {
        dir = "/tmp";
    }

    const auto stamp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    fs::path p = dir / (baseName + "_" + std::to_string(stamp) + ".save");
    return p.string();
}

TEST(CommandsCpp, a3)
{
    FileSystem fs;
    addGroupCommand cmd;

    EXPECT_THROW(cmd.execute(Command{"addgroup"}, fs), std::invalid_argument);
}

TEST(CommandsCpp, a4)
{
    FileSystem fs;
    addGroupCommand cmd;

    const std::string out = cmd.execute(Command{"addgroup devs"}, fs);
    EXPECT_NE(out.find("Group added, GID = "), std::string::npos);

    EXPECT_NE(fs.getGroup(1), nullptr);
    EXPECT_EQ(fs.getGroup(1)->name_, "devs");
}

TEST(CommandsCpp, a5)
{
    FileSystem fs;
    addUserCommand cmd;

    EXPECT_THROW(cmd.execute(Command{"adduser"}, fs), std::invalid_argument);
}

TEST(CommandsCpp, a6)
{
    FileSystem fs;
    addUserCommand cmd;

    const std::string out = cmd.execute(Command{"adduser alice"}, fs);
    EXPECT_NE(out.find("User added, UID = "), std::string::npos);

    EXPECT_NE(fs.getUser(1), nullptr);
    EXPECT_EQ(fs.getUser(1)->name_, "alice");
}

TEST(CommandsCpp, a7)
{
    FileSystem fs;
    deleteGroupCommand cmd;

    EXPECT_THROW(cmd.execute(Command{"deletegroup not_a_number"}, fs), std::invalid_argument);
}

TEST(CommandsCpp, a8)
{
    FileSystem fs;
    deleteGroupCommand cmd;

    EXPECT_THROW(cmd.execute(Command{"deletegroup 9999"}, fs), std::invalid_argument);
}

TEST(CommandsCpp, a9)
{
    FileSystem fs;
    addGroupCommand add;
    deleteGroupCommand del;

    add.execute(Command{"addgroup g1"}, fs);
    ASSERT_NE(fs.getGroup(1), nullptr);

    const std::string out = del.execute(Command{"deletegroup 1"}, fs);
    EXPECT_NE(out.find("Group deleted, GID = 1"), std::string::npos);
    EXPECT_EQ(fs.getGroup(1), nullptr);
}

TEST(CommandsCpp, a11)
{
    FileSystem fs;
    deleteUserCommand cmd;

    EXPECT_THROW(cmd.execute(Command{"deleteuser xxx"}, fs), std::invalid_argument);
}

TEST(CommandsCpp, s1)
{
    FileSystem fs;
    deleteUserCommand cmd;

    EXPECT_THROW(cmd.execute(Command{"deleteuser 9999"}, fs), std::invalid_argument);
}

TEST(CommandsCpp, s2)
{
    FileSystem fs;
    addUserCommand add;
    deleteUserCommand del;

    add.execute(Command{"adduser bob"}, fs);
    ASSERT_NE(fs.getUser(1), nullptr);

    const std::string out = del.execute(Command{"deleteuser 1"}, fs);
    EXPECT_NE(out.find("User deleted, UID = 1"), std::string::npos);
    EXPECT_EQ(fs.getUser(1), nullptr);
}

TEST(CommandsCpp, s3)
{
    FileSystem fs;

    addGroupCommand addG;
    addUserCommand addU;
    addG.execute(Command{"addgroup g1"}, fs);
    addU.execute(Command{"adduser u1"}, fs);

    ListGroupsCommand lsg;
    ListUsersCommand lsu;

    EXPECT_EQ(lsg.execute(Command{"lsgroups"}, fs), fs.ListGroups());
    EXPECT_EQ(lsu.execute(Command{"lsusers"}, fs), fs.ListUsers());
}

TEST(CommandsCpp, s4)
{
    const std::string savePath = makeTempPath("commands_cpp_roundtrip");

    FileSystem fs;

    auto root = fs.getRoot();
    ASSERT_TRUE(root);

    {
        const uint64_t id = fs.getNextFileID();
        auto a = std::make_shared<RegularFile>(root, id, "a", fs.getCurrentUser());
        a->setData("hello");
        root->addFile(id, a);
    }

    {
        const uint64_t did = fs.getNextFileID();
        auto d = std::make_shared<Directory>(root, did, "d", fs.getCurrentUser());
        root->addFile(did, d);

        const uint64_t cid = fs.getNextFileID();
        auto c = std::make_shared<RegularFile>(d, cid, "c", fs.getCurrentUser());
        c->setData("inside\n");
        d->addFile(cid, c);
    }

    SaveSystem save;
    const std::string outSave = save.execute(Command{"save " + savePath}, fs);
    EXPECT_NE(outSave.find("Saved to " + savePath), std::string::npos);
    ASSERT_TRUE(std::filesystem::exists(savePath));

    FileSystem loadedFs;
    LoadSystem load;
    const std::string outLoad = load.execute(Command{"load " + savePath}, loadedFs);
    EXPECT_NE(outLoad.find("Loaded from " + savePath), std::string::npos);

    auto loadedRoot = loadedFs.getRoot();
    ASSERT_TRUE(loadedRoot);

    {
        auto f = loadedRoot->findByName("a");
        ASSERT_TRUE(f);
        auto reg = std::dynamic_pointer_cast<RegularFile>(f);
        ASSERT_TRUE(reg);
        EXPECT_EQ(reg->getData(), "hello\n");
    }

    {
        auto d = std::dynamic_pointer_cast<Directory>(loadedRoot->findByName("d"));
        ASSERT_TRUE(d);

        auto f = d->findByName("c");
        ASSERT_TRUE(f);
        auto reg = std::dynamic_pointer_cast<RegularFile>(f);
        ASSERT_TRUE(reg);
        EXPECT_EQ(reg->getData(), "inside\n");
    }

    std::error_code ec;
    std::filesystem::remove(savePath, ec);
}

TEST(CommandsCpp, s5)
{
    FileSystem fs;
    LoadSystem load;

    EXPECT_THROW(load.execute(Command{"load definitely_not_exists_12345.save"}, fs), std::runtime_error);
}

TEST(CommandsCpp, ss1)
{
    const std::string p = makeTempPath("commands_cpp_bad_start");

    {
        std::ofstream out(p);
        out << "touch a\n";
        out << "WRONG_MARKER\n";
    }

    FileSystem fs;
    LoadSystem load;
    EXPECT_THROW(load.execute(Command{"load " + p}, fs), std::runtime_error);

    std::error_code ec;
    std::filesystem::remove(p, ec);
}

TEST(CommandsCpp, z1)
{
    const std::string p = makeTempPath("commands_cpp_bad_end");

    {
        std::ofstream out(p);
        out << "touch a\n";
        out << "<start>\n";
        out << "hello\n";
    }

    FileSystem fs;
    LoadSystem load;
    EXPECT_THROW(load.execute(Command{"load " + p}, fs), std::runtime_error);

    std::error_code ec;
    std::filesystem::remove(p, ec);
}


TEST(Command, z2) {
    Command c{""};
    EXPECT_TRUE(c.name.empty());
    EXPECT_TRUE(c.args.empty());
}

TEST(Command, z3) {
    Command c{"touch a.txt"};
    EXPECT_EQ(c.name, "touch");
    ASSERT_EQ(c.args.size(), 1u);
    EXPECT_EQ(c.args[0], "a.txt");
}

TEST(Command, z4) {
    Command c{"rename old new"};
    EXPECT_EQ(c.name, "rename");
    ASSERT_EQ(c.args.size(), 2u);
    EXPECT_EQ(c.args[0], "old");
    EXPECT_EQ(c.args[1], "new");
}
