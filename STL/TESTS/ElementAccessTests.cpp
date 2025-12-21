#include "gtest/gtest.h"
#include "table.hpp"
#include <string>

using sstd::table;

TEST(ElementAccessTest, Operator) {
    table<int, std::string> t;
    t[1] = "one";
    EXPECT_EQ(t[1], "one");
    t[2] = "two";
    EXPECT_EQ(t[2], "two");
    t[1] = "ONE";
    EXPECT_EQ(t[1], "ONE");
}

TEST(ElementAccessTest, Operator2) {
    table<int, std::string> t;
    const auto& val = t[100];
    EXPECT_EQ(val, std::string{});
    EXPECT_TRUE(t.contains(100));
    EXPECT_EQ(t.size(), 1);
}

TEST(ElementAccessTest, Operator3) {
    table<int, int> t;
    t[10] = 42;
    t[10] += 1;
    EXPECT_EQ(t[10], 43);
}

TEST(ElementAccessTest, At1) {
    table<int, int> t;
    t[5] = 99;
    int& ref = t.at(5);
    EXPECT_EQ(ref, 99);
    ref = 100;
    EXPECT_EQ(t[5], 100);
}

TEST(ElementAccessTest, At2) {
    table<int, std::string> t;
    EXPECT_THROW(t.at(123), std::out_of_range);
}

TEST(ElementAccessTest, At3) {
    table<int, std::string> t;
    t[1] = "abc";
    const table<int, std::string>& ct = t;
    const std::string& ref = ct.at(1);
    EXPECT_EQ(ref, "abc");
}

TEST(ElementAccessTest, At4) {
    table<int, int> t;
    const table<int, int>& ct = t;
    EXPECT_THROW(auto c = ct.at(555), std::out_of_range);
}

TEST(ElementAccessTest, At5) {
    table<int, int> t;
    t[7] = 77;
    t.at(7) = 88;
    EXPECT_EQ(t[7], 88);
    t[7] = 99;
    EXPECT_EQ(t.at(7), 99);
}
