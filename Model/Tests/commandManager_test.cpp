#include <gtest/gtest.h>

#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>

#include "commandManager.hpp"
#include "filesys.hpp"

TEST(CommandManager, a1)
{
    const std::string soPath =
        "/mnt/d/Codes/OOP/3/project/build/cFind/libreplace.so";

    FileSystem fs;
    CommandManager mgr;

    const std::string out = mgr.executeCommand(fs, "install " + soPath);

    ASSERT_TRUE(out.rfind("Installed: ", 0) == 0);
    const std::string installedName = out.substr(std::string("Installed: ").size());
    ASSERT_FALSE(installedName.empty());

    const std::string help = mgr.help();
    EXPECT_NE(help.find(installedName), std::string::npos);

}





TEST(CommandManager, a2)
{
    CommandManager mgr;
    const auto cmds = mgr.getCmds();

}

TEST(CommandManager, a3)
{
    FileSystem fs;
    CommandManager mgr;

    EXPECT_TRUE(mgr.executeCommand(fs, "").empty());
}

TEST(CommandManager, a4)
{
    FileSystem fs;
    CommandManager mgr;

    EXPECT_THROW(mgr.executeCommand(fs, "definitely_unknown_cmd"), std::invalid_argument);
}

TEST(CommandManager, a5)
{
    FileSystem fs;
    CommandManager mgr;

    EXPECT_NO_THROW(mgr.executeCommand(fs, "touch a"));

    auto cur = fs.getCurrentDirectory();
    ASSERT_TRUE(cur);
    ASSERT_TRUE(cur->findByName("a"));

    const auto text = mgr.executeCommand(fs, "open a");
    EXPECT_TRUE(text.empty());
}

TEST(CommandManager, a6)
{
    FileSystem fs;
    CommandManager mgr;

    EXPECT_NO_THROW(mgr.executeCommand(fs, "alias t touch"));
    EXPECT_NO_THROW(mgr.executeCommand(fs, "t b"));

    auto cur = fs.getCurrentDirectory();
    ASSERT_TRUE(cur);
    EXPECT_TRUE(cur->findByName("b") != nullptr);

    const auto help = mgr.help();
    EXPECT_NE(help.find("t"), std::string::npos);
}

TEST(CommandManager, a7)
{
    FileSystem fs;
    CommandManager mgr;

    EXPECT_NO_THROW(mgr.addCommand("tt", "touch"));
    EXPECT_NO_THROW(mgr.executeCommand(fs, "tt c"));

    auto cur = fs.getCurrentDirectory();
    ASSERT_TRUE(cur);
    EXPECT_TRUE(cur->findByName("c") != nullptr);

    mgr.deleteCommand("tt");
    EXPECT_THROW(mgr.executeCommand(fs, "tt d"), std::invalid_argument);
}

TEST(CommandManager, a8)
{
    FileSystem fs;
    CommandManager mgr;

    EXPECT_THROW(mgr.executeCommand(fs, "install"), std::invalid_argument);
}