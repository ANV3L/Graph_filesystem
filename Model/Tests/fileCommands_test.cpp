#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <string>

#include "FileCommands.hpp"
#include "filesys.hpp"
#include "files.hpp"
#include "command.hpp"
#include "RegularFileCommands.hpp"
#include "DirectoryCommands.hpp"

TEST(RegularFileCommands, s2)
{
    FileSystem fs;
    openReadFileCommand open;

    EXPECT_THROW(open.execute(Command{"open"}, fs), std::invalid_argument);
    EXPECT_THROW(open.execute(Command{"open a b"}, fs), std::invalid_argument);
}

TEST(RegularFileCommands, s5)
{
    FileSystem fs;
    fs.setCurrentDirectory(nullptr);

    openReadFileCommand open;
    EXPECT_THROW(open.execute(Command{"open a"}, fs), std::runtime_error);
}

TEST(RegularFileCommands, s3)
{
    FileSystem fs;
    openReadFileCommand open;

    EXPECT_THROW(open.execute(Command{"open missing"}, fs), std::invalid_argument);
}

TEST(RegularFileCommands, s4)
{
    FileSystem fs;

    CreateDirectoryCommand mkdir;
    ASSERT_NO_THROW(mkdir.execute(Command{"mkdir d"}, fs));

    openReadFileCommand open;
    EXPECT_THROW(open.execute(Command{"open d"}, fs), std::invalid_argument);
}

TEST(RegularFileCommands, s6)
{
    FileSystem fs;

    auto cur = fs.getCurrentDirectory();
    ASSERT_TRUE(cur);

    const uint64_t id = fs.getNextFileID();
    auto rf = std::make_shared<RegularFile>(cur, id, "a", fs.getCurrentUser());
    rf->setData("hello");
    cur->addFile(id, rf);

    openReadFileCommand open;
    EXPECT_EQ(open.execute(Command{"open a"}, fs), "hello");
}

TEST(RegularFileCommands, s8)
{
    FileSystem fs;
    fs.setCurrentDirectory(nullptr);

    ListRegularFiles lsReg;
    EXPECT_THROW(lsReg.execute(Command{"lsRegular"}, fs), std::runtime_error);
}

TEST(RegularFileCommands, s1)
{
    FileSystem fs;
    auto cur = fs.getCurrentDirectory();
    ASSERT_TRUE(cur);

    {
        const uint64_t id = fs.getNextFileID();
        auto rf = std::make_shared<RegularFile>(cur, id, "r1", fs.getCurrentUser());
        cur->addFile(id, rf);
    }

    {
        const uint64_t id = fs.getNextFileID();
        auto d = std::make_shared<Directory>(cur, id, "d1", fs.getCurrentUser());
        cur->addFile(id, d);
    }

    {
        const uint64_t id = fs.getNextFileID();
        auto rf = std::make_shared<RegularFile>(cur, id, "r2", fs.getCurrentUser());
        cur->addFile(id, rf);
    }

    ListRegularFiles lsReg;
    const std::string out = lsReg.execute(Command{"lsRegular"}, fs);

    EXPECT_NE(out.find("r1"), std::string::npos);
    EXPECT_NE(out.find("r2"), std::string::npos);
    EXPECT_EQ(out.find("d1"), std::string::npos);
    EXPECT_NE(out.find(' '), std::string::npos);
}


TEST(FileCommands, a1)
{
    FileSystem fs;
    CreateFileCommand touch;

    EXPECT_THROW(touch.execute(Command{"touch"}, fs), std::invalid_argument);
}

TEST(FileCommands, a2)
{
    FileSystem fs;
    fs.setCurrentDirectory(nullptr);

    CreateFileCommand touch;
    EXPECT_THROW(touch.execute(Command{"touch a"}, fs), std::runtime_error);
}

TEST(FileCommands, s0)
{
    FileSystem fs;
    CreateFileCommand touch;

    EXPECT_NO_THROW(touch.execute(Command{"touch a"}, fs));
    EXPECT_THROW(touch.execute(Command{"touch a"}, fs), std::runtime_error);
}


TEST(FileCommands, l1)
{
    FileSystem fs;
    RenameFileCommand rn;

    EXPECT_THROW(rn.execute(Command{"rename onlyone"}, fs), std::invalid_argument);
}

TEST(FileCommands, o03)
{
    FileSystem fs;
    RenameFileCommand rn;

    Command cmd{"rename a b"};
    cmd.args[1].clear();

    EXPECT_THROW(rn.execute(cmd, fs), std::invalid_argument);
}

TEST(FileCommands, z3)
{
    FileSystem fs;
    fs.setCurrentDirectory(nullptr);

    RenameFileCommand rn;
    EXPECT_THROW(rn.execute(Command{"rename a b"}, fs), std::runtime_error);
}

TEST(FileCommands, c4)
{
    FileSystem fs;
    RenameFileCommand rn;

    EXPECT_NO_THROW(rn.execute(Command{"rename . newhome"}, fs));
    ASSERT_TRUE(fs.getCurrentDirectory());
    EXPECT_EQ(fs.getCurrentDirectory()->getName(), "newhome");
}

TEST(FileCommands, k1)
{
    FileSystem fs;
    auto root = fs.getRoot();
    ASSERT_TRUE(root);

    const uint64_t did = fs.getNextFileID();
    auto d = std::make_shared<Directory>(root, did, "d", fs.getCurrentUser());
    root->addFile(did, d);

    const uint64_t xid = fs.getNextFileID();
    auto x = std::make_shared<RegularFile>(root, xid, "x", fs.getCurrentUser());
    root->addFile(xid, x);

    fs.setCurrentDirectory(d);

    RenameFileCommand rn;
    EXPECT_THROW(rn.execute(Command{"rename . x"}, fs), std::runtime_error);
}

TEST(FileCommands, l0)
{
    FileSystem fs;
    CreateFileCommand touch;
    RenameFileCommand rn;

    touch.execute(Command{"touch a"}, fs);
    touch.execute(Command{"touch b"}, fs);

    EXPECT_THROW(rn.execute(Command{"rename a b"}, fs), std::runtime_error);
}

TEST(FileCommands, m9)
{
    FileSystem fs;
    RenameFileCommand rn;

    EXPECT_THROW(rn.execute(Command{"rename no_such_file x"}, fs), std::invalid_argument);
}

TEST(FileCommands, m99)
{
    FileSystem fs;
    RemoveFileCommand rm;

    EXPECT_THROW(rm.execute(Command{"rm"}, fs), std::invalid_argument);
}

TEST(FileCommands, vv)
{
    FileSystem fs;
    fs.setCurrentDirectory(nullptr);

    RemoveFileCommand rm;
    EXPECT_THROW(rm.execute(Command{"rm a"}, fs), std::runtime_error);
}

TEST(FileCommands, vvv)
{
    FileSystem fs;
    RemoveFileCommand rm;

    EXPECT_THROW(rm.execute(Command{"rm nofile"}, fs), std::invalid_argument);
}


TEST(FileCommands, x)
{
    FileSystem fs;
    ChmodFileCommand chmod;

    EXPECT_THROW(chmod.execute(Command{"chmod a"}, fs), std::invalid_argument);
}

TEST(FileCommands, xx)
{
    FileSystem fs;
    fs.setCurrentDirectory(nullptr);

    ChmodFileCommand chmod;
    EXPECT_THROW(chmod.execute(Command{"chmod a 777"}, fs), std::runtime_error);
}

TEST(FileCommands, xxx)
{
    FileSystem fs;
    ChmodFileCommand chmod;

    EXPECT_THROW(chmod.execute(Command{"chmod nofile 777"}, fs), std::invalid_argument);
}

TEST(FileCommands, z111)
{
    FileSystem fs;
    CreateFileCommand touch;
    ChmodFileCommand chmod;

    touch.execute(Command{"touch a"}, fs);

    EXPECT_THROW(chmod.execute(Command{"chmod a not_octal"}, fs), std::invalid_argument);
}

TEST(FileCommands, zaq) {
    FileSystem fs;

    CreateFileCommand touch;
    auto res = touch.execute(Command{"touch a"}, fs);
    EXPECT_NE(res.find("File created"), std::string::npos);

    auto cur = fs.getCurrentDirectory();
    ASSERT_TRUE(cur);
    ASSERT_TRUE(cur->findByName("a"));
}

TEST(FileCommands, sq) {
    FileSystem fs;
    CreateFileCommand touch;
    touch.execute(Command{"touch a"}, fs);

    RenameFileCommand rn;
    EXPECT_NO_THROW(rn.execute(Command{"rename a b"}, fs));

    auto cur = fs.getCurrentDirectory();
    ASSERT_TRUE(cur);
    EXPECT_FALSE(cur->findByName("a"));
    EXPECT_TRUE(cur->findByName("b"));
}

TEST(FileCommands, bnjip) {
    FileSystem fs;
    CreateFileCommand touch;
    touch.execute(Command{"touch a"}, fs);

    RemoveFileCommand rm;
    auto out = rm.execute(Command{"rm a"}, fs);
    EXPECT_NE(out.find("File deleted"), std::string::npos);

    auto cur = fs.getCurrentDirectory();
    ASSERT_TRUE(cur);
    EXPECT_FALSE(cur->findByName("a"));
}

TEST(FileCommands, cefj) {
    FileSystem fs;
    CreateFileCommand touch;
    touch.execute(Command{"touch a"}, fs);

    ChmodFileCommand chmod;
    EXPECT_NO_THROW(chmod.execute(Command{"chmod a 777"}, fs));

    auto cur = fs.getCurrentDirectory();
    auto f = cur->findByName("a");
    ASSERT_TRUE(f);
    EXPECT_EQ(f->getAccessRules(), 0777);
}
