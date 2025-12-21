#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include <string>

#include "DirectoryCommands.hpp"
#include "FileCommands.hpp"
#include "command.hpp"
#include "filesys.hpp"
#include "files.hpp"

TEST(DirectoryCommands, q1)
{
    FileSystem fs;
    fs.setCurrentDirectory(nullptr);

    ListDirectoryCommand ls;
    EXPECT_THROW(ls.execute(Command{"ls"}, fs), std::runtime_error);
}

TEST(DirectoryCommands, q2)
{
    FileSystem fs;

    CreateDirectoryCommand mkdir;
    ChangeDirectory cd;
    PrintWorkingDirectory pwd;

    ASSERT_NO_THROW(mkdir.execute(Command{"mkdir dir1"}, fs));
    ASSERT_NO_THROW(cd.execute(Command{"cd dir1"}, fs));
    ASSERT_NO_THROW(mkdir.execute(Command{"mkdir dir2"}, fs));
    ASSERT_NO_THROW(cd.execute(Command{"cd dir2"}, fs));

    const std::string out = pwd.execute(Command{"pwd"}, fs);
    EXPECT_EQ(out, "home\\dir1\\dir2");
}

TEST(DirectoryCommands, q3)
{
    FileSystem fs;
    CreateDirectoryCommand mkdir;

    EXPECT_THROW(mkdir.execute(Command{"mkdir"}, fs), std::invalid_argument);
}

TEST(DirectoryCommands, a4)
{
    FileSystem fs;
    fs.setCurrentDirectory(nullptr);

    CreateDirectoryCommand mkdir;
    EXPECT_THROW(mkdir.execute(Command{"mkdir x"}, fs), std::runtime_error);
}

TEST(DirectoryCommands, a1)
{
    FileSystem fs;
    CreateDirectoryCommand mkdir;

    ASSERT_NO_THROW(mkdir.execute(Command{"mkdir d"}, fs));
    EXPECT_THROW(mkdir.execute(Command{"mkdir d"}, fs), std::runtime_error);
}

TEST(DirectoryCommands, a2)
{
    FileSystem fs;
    ChangeDirectory cd;

    EXPECT_THROW(cd.execute(Command{"cd"}, fs), std::invalid_argument);
}

TEST(DirectoryCommands, a3)
{
    FileSystem fs;
    fs.setCurrentDirectory(nullptr);

    ChangeDirectory cd;
    EXPECT_THROW(cd.execute(Command{"cd x"}, fs), std::runtime_error);
}

TEST(DirectoryCommands, z1)
{
    FileSystem fs;
    ChangeDirectory cd;

    EXPECT_EQ(cd.execute(Command{"cd .."}, fs), "");
}

TEST(DirectoryCommands, z2)
{
    FileSystem fs;
    ChangeDirectory cd;

    EXPECT_THROW(cd.execute(Command{"cd missing"}, fs), std::invalid_argument);
}

TEST(DirectoryCommands, z3)
{
    FileSystem fs;

    CreateFileCommand touch;
    ChangeDirectory cd;

    ASSERT_NO_THROW(touch.execute(Command{"touch a"}, fs));
    EXPECT_THROW(cd.execute(Command{"cd a"}, fs), std::invalid_argument);
}



TEST(DirectoryCommands, z4) {
    FileSystem fs;

    CreateDirectoryCommand mkdirCmd;
    Command mkdirLine{"mkdir dir1"};
    auto res = mkdirCmd.execute(mkdirLine, fs);
    EXPECT_NE(res.find("Directory created"), std::string::npos);

    ListDirectoryCommand lsCmd;
    Command lsLine{"ls"};
    auto ls = lsCmd.execute(lsLine, fs);
    EXPECT_NE(ls.find("dir1"), std::string::npos);
}

TEST(DirectoryCommands, z5) {
    FileSystem fs;

    CreateDirectoryCommand mkdirCmd;
    mkdirCmd.execute(Command{"mkdir d"}, fs);

    ChangeDirectory cd;
    EXPECT_NO_THROW(cd.execute(Command{"cd d"}, fs));
    EXPECT_EQ(fs.getCurrentDirectory()->getName(), "d");

    EXPECT_NO_THROW(cd.execute(Command{"cd .."}, fs));
    EXPECT_EQ(fs.getCurrentDirectory()->getName(), "home");
}

TEST(DirectoryCommands, z6) {
    FileSystem fs;
    PrintWorkingDirectory pwd;
    auto path = pwd.execute(Command{"pwd"}, fs);
    EXPECT_FALSE(path.empty());
}
