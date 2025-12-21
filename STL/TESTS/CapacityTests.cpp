#include "gtest/gtest.h"
#include "table.hpp"
#include <string>
#include <limits>

using sstd::table;

TEST(CapacityTest, Empty1) {
    const table<int, std::string> t;

    EXPECT_TRUE(t.empty());
}

TEST(CapacityTest, Empty2) {
    const table<int, int> t = {{1, 10}};

    EXPECT_FALSE(t.empty());
}

TEST(CapacityTest, Size1) {
    const table<int, std::string> t;

    EXPECT_EQ(t.size(), 0);
}

TEST(CapacityTest, Size2) {
    const table<int, int> t = {{1, 10}};

    EXPECT_EQ(t.size(), 1);
}

TEST(CapacityTest, Size3) {
    const table<int, int> t = {{1, 10}, {2, 20}, {3, 30}};

    EXPECT_EQ(t.size(), 3);
}

TEST(CapacityTest, MaxSize) {
    const table<int, int> t;

    EXPECT_EQ(t.max_size(), std::numeric_limits<std::size_t>::max());
}