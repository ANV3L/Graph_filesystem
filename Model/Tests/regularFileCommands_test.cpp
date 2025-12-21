#include <gtest/gtest.h>
#include <memory>

#include "RegularFileCommands.hpp"
#include "FileCommands.hpp"
#include "filesys.hpp"
#include "files.hpp"
#include "command.hpp"
#include "DirectoryCommands.hpp"

TEST(RegularFileCommands, q1) {
    FileSystem fs;

    CreateFileCommand touch;
    touch.execute(Command{"touch a"}, fs);

    auto cur = fs.getCurrentDirectory();
    auto f = cur->findByName("a");
    auto reg = std::dynamic_pointer_cast<RegularFile>(f);
    ASSERT_TRUE(reg);
    reg->setData("hello");

    openReadFileCommand open;
    auto text = open.execute(Command{"open a"}, fs);
    EXPECT_EQ(text, "hello");
}

TEST(RegularFileCommands, q2) {
    FileSystem fs;

    CreateFileCommand touch;
    touch.execute(Command{"touch f1"}, fs);
    touch.execute(Command{"touch f2"}, fs);

    CreateDirectoryCommand mkdir;
    mkdir.execute(Command{"mkdir d1"}, fs);

    ListRegularFiles lsReg;
    auto out = lsReg.execute(Command{"lsRegular"}, fs);

    EXPECT_NE(out.find("f1"), std::string::npos);
    EXPECT_NE(out.find("f2"), std::string::npos);
    EXPECT_EQ(out.find("d1"), std::string::npos);
}
