#include "gtest/gtest.h"
#include "table.hpp"
#include <string>
#include <vector>

using sstd::table;

TEST(ComparisonTest, Equal) {
    table<int, std::string> t1 = {{1, "one"}, {2, "two"}};
    table<int, std::string> t2 = {{2, "two"}, {1, "one"}};
    EXPECT_TRUE(t1 == t2);
}

TEST(ComparisonTest, NotEqual) {
    table<int, int> t1 = {{1, 10}, {2, 20}};
    table<int, int> t2 = {{1, 10}};
    EXPECT_FALSE(t1 == t2);
}

TEST(ComparisonTest, NotEqual2) {
    table<int, int> t1 = {{1, 10}};
    table<int, int> t2 = {{1, 100}};
    EXPECT_FALSE(t1 == t2);
}

TEST(ComparisonTest, Equal2) {
    table<int, std::string> t1, t2;
    EXPECT_TRUE(t1 == t2);
}


TEST(ComparisonTest, Spaceship1) {
    table<int, int> t1 = {{1, 10}, {2, 20}, {5, 50}};
    table<int, int> t2 = {{1, 10}, {2, 20}, {4, 40}};
    EXPECT_TRUE((t1 <=> t2) > 0);
}

TEST(ComparisonTest, Spaceship2) {
    table<int, int> t1 = {{1, 10}, {2, 5}};
    table<int, int> t2 = {{1, 10}, {2, 7}};
    EXPECT_TRUE((t1 <=> t2) < 0);
}

TEST(ComparisonTest, Swap1) {
    table<int, std::string> t1 = {{1, "one"}, {2, "two"}};
    table<int, std::string> t2 = {{100, "xxx"}};
    swap(t1, t2);

    EXPECT_EQ(t1.size(), 1);
    EXPECT_EQ(t1.at(100), "xxx");
    EXPECT_EQ(t2.size(), 2);
    EXPECT_EQ(t2.at(1), "one");
    EXPECT_EQ(t2.at(2), "two");
}

TEST(ComparisonTest, Swap2) {
    table<int, int> t = {{1, 10}, {2, 20}};
    swap(t, t);
    EXPECT_EQ(t.size(), 2);
    EXPECT_EQ(t.at(1), 10);
    EXPECT_EQ(t.at(2), 20);
}

TEST(ComparisonTest, EraseIf1) {
    table<int, int> t = {{1, 1}, {2, 2}, {3, 3}, {4, 4}};
    auto erased = erase_if(t, [](const auto& pair){ return pair.first % 2 == 1; });
    EXPECT_EQ(erased, 2);
    EXPECT_EQ(t.size(), 2);
    EXPECT_FALSE(t.contains(1));
    EXPECT_TRUE(t.contains(2));
    EXPECT_FALSE(t.contains(3));
    EXPECT_TRUE(t.contains(4));
}

TEST(ComparisonTest, EraseIf2) {
    table<std::string, int> t = {{"a", 1}, {"b", 2}};
    auto erased = erase_if(t, [](const auto&){ return false; });
    EXPECT_EQ(erased, 0);
    EXPECT_EQ(t.size(), 2);
}

TEST(ComparisonTest, EraseIf3) {
    table<int, int> t = {{1,1}, {2,2}};
    auto erased = erase_if(t, [](const auto&){ return true; });
    EXPECT_EQ(erased, 2);
    EXPECT_TRUE(t.empty());
}
