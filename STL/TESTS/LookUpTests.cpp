#include "gtest/gtest.h"
#include "table.hpp"
#include <string>
#include <vector>

using sstd::table;

TEST(LookupTest, Count1) {
    const table<int, std::string> t = {{1, "one"}, {2, "two"}};
    EXPECT_EQ(t.count(1), 1);
    EXPECT_EQ(t.count(2), 1);
}

TEST(LookupTest, Count2) {
    const table<int, int> t = {{1, 10}, {2, 20}};
    EXPECT_EQ(t.count(0), 0);
    EXPECT_EQ(t.count(3), 0);
}

TEST(LookupTest, Count3) {
    const table<int, int> t;
    EXPECT_EQ(t.count(5), 0);
}

TEST(LookupTest, Find1) {
    table<int, std::string> t = {{1, "one"}, {2, "two"}};
    auto it = t.find(2);
    ASSERT_NE(it, t.end());
    EXPECT_EQ(it->first, 2);
    EXPECT_EQ(it->second, "two");
}

TEST(LookupTest, Find2) {
    table<int, int> t = {{1, 10}};
    auto it = t.find(5);
    EXPECT_EQ(it, t.end());
}

TEST(LookupTest, Find3_1) {
    table<std::string, int> tmp = {{"cat", 10}, {"good", 20}};
    const table<std::string, int>& t = tmp;
    auto it = t.find("cat");
    EXPECT_NE(it, t.end());
    EXPECT_EQ(it->first, "cat");
    EXPECT_EQ(it->second, 10);
}

TEST(LookupTest, Find3) {
    table<int, int> tmp = {{1, 10}, {2, 20}};
    const table<int, int>& t = tmp;
    auto it = t.find(1);
    ASSERT_NE(it, t.end());
    EXPECT_EQ(it->first, 1);
    EXPECT_EQ(it->second, 10);
}

TEST(LookupTest, Find4) {
    table<int, int> tmp = {{1, 10}};
    const table<int, int>& t = tmp;
    auto it = t.find(2);
    EXPECT_EQ(it, t.end());
}

TEST(LookupTest, Contains1) {
    table<int, std::string> t = {{1, "one"}, {2, "two"}};
    EXPECT_TRUE(t.contains(1));
    EXPECT_TRUE(t.contains(2));
}

TEST(LookupTest, Contains2) {
    table<int, int> t = {{1, 10}};
    EXPECT_FALSE(t.contains(0));
    EXPECT_FALSE(t.contains(2));
}

TEST(LookupTest, Contains3) {
    table<int, int> t;
    EXPECT_FALSE(t.contains(5));
}

TEST(LookupTest, EqualRange1) {
    table<int, int> t = {{1, 10}, {2, 20}, {3, 30}};
    auto range = t.equal_range(2);
    ASSERT_NE(range.first, t.end());
    EXPECT_EQ(range.first->first, 2);
    auto it_next = range.first;
    ++it_next;
    EXPECT_EQ(range.second, it_next);
}

TEST(LookupTest, EqualRange2) {
    table<int, int> t = {{10, 100}, {20, 200}, {30, 300}};
    auto [fst, snd] = t.equal_range(5);

    ASSERT_NE(fst, t.end());
    EXPECT_EQ(fst->first, 10);

    EXPECT_EQ(fst, snd);
}

TEST(LookupTest, EqualRange3) {
    table<int, int> t = {{10, 100}, {20, 200}, {30, 300}};
    auto range = t.equal_range(40);

    EXPECT_EQ(range.first, t.end());
    EXPECT_EQ(range.second, t.end());
}

TEST(LookupTest, EqualRange4) {
    table<int, int> tmp = {{1, 10}, {3, 30}, {5, 50}};
    const table<int, int>& t = tmp;
    auto range = t.equal_range(3);
    ASSERT_NE(range.first, t.end());
    EXPECT_EQ(range.first->first, 3);
    auto it_next = range.first;
    ++it_next;
    EXPECT_EQ(range.second, it_next);
}

TEST(LookupTest, LowerBound1) {
    table<int, int> t = {{1, 10}, {3, 30}, {5, 50}};
    auto it = t.lower_bound(3);
    ASSERT_NE(it, t.end());
    EXPECT_EQ(it->first, 3);
}

TEST(LookupTest, LowerBound2) {
    table<int, int> t = {{10, 100}, {20, 200}, {30, 300}};
    auto it = t.lower_bound(15);
    ASSERT_NE(it, t.end());
    EXPECT_EQ(it->first, 20);
}

TEST(LookupTest, LowerBound3) {
    table<int, int> t = {{10, 100}, {20, 200}};
    auto it = t.lower_bound(0);
    ASSERT_NE(it, t.end());
    EXPECT_EQ(it->first, 10);
}

TEST(LookupTest, LowerBound4) {
    table<int, int> t = {{10, 100}, {20, 200}};
    auto it = t.lower_bound(30);
    EXPECT_EQ(it, t.end());
}

TEST(LookupTest, LowerBound5) {
    table<int, int> tmp = {{1, 10}, {4, 40}, {7, 70}};
    const table<int, int>& t = tmp;
    auto it = t.lower_bound(5);
    ASSERT_NE(it, t.end());
    EXPECT_EQ(it->first, 7);
}

TEST(LookupTest, UpperBound1) {
    table<int, int> t = {{1, 10}, {3, 30}, {5, 50}};
    auto it = t.upper_bound(3);
    ASSERT_NE(it, t.end());
    EXPECT_EQ(it->first, 5);
}

TEST(LookupTest, UpperBound2) {
    table<int, int> t = {{10, 100}, {20, 200}, {30, 300}};
    auto it = t.upper_bound(15);
    ASSERT_NE(it, t.end());
    EXPECT_EQ(it->first, 20);
}

TEST(LookupTest, UpperBound3) {
    table<int, int> t = {{10, 100}, {20, 200}};
    auto it = t.upper_bound(0);
    ASSERT_NE(it, t.end());
    EXPECT_EQ(it->first, 10);
}

TEST(LookupTest, UpperBound4) {
    table<int, int> t = {{10, 100}, {20, 200}};
    auto it = t.upper_bound(30);
    EXPECT_EQ(it, t.end());
}

TEST(LookupTest, UpperBound5) {
    table<int, int> tmp = {{1, 10}, {4, 40}, {7, 70}};
    const table<int, int>& t = tmp;
    auto it = t.upper_bound(4);
    ASSERT_NE(it, t.end());
    EXPECT_EQ(it->first, 7);
}
