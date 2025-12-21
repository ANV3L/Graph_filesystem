#include "gtest/gtest.h"
#include "table.hpp"
#include <string>
#include <functional>

using sstd::table;

TEST(ComparatorsTest, Key_comp1) {
    table<int, std::string> t;
    auto cmp = t.key_comp();

    EXPECT_TRUE(cmp(1, 2));
    EXPECT_FALSE(cmp(2, 1));
    EXPECT_FALSE(cmp(5, 5));
}

TEST(ComparatorsTest, Key_comp2) {
    table<int, int, std::greater<>> t;
    auto cmp = t.key_comp();

    EXPECT_TRUE(cmp(2, 1));
    EXPECT_FALSE(cmp(1, 2));
    EXPECT_FALSE(cmp(5, 5));
}

TEST(ComparatorsTest, value_comp1) {
    table<int, std::string> t;
    auto cmp = t.value_comp();

    std::pair<const int, std::string> p1{1, "one"};
    std::pair<const int, std::string> p2{2, "two"};

    EXPECT_TRUE(cmp(p1, p2));
    EXPECT_FALSE(cmp(p2, p1));
}

TEST(ComparatorsTest, value_comp2) {
    table<int, int> t;
    auto cmp = t.value_comp();

    std::pair<const int, int> p1{5, 100};
    std::pair<const int, int> p2{5, 999};

    EXPECT_FALSE(cmp(p1, p2));
    EXPECT_FALSE(cmp(p2, p1));
}