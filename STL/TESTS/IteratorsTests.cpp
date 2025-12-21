#include "gtest/gtest.h"
#include "table.hpp"
#include <string>
#include <vector>

using sstd::table;
#include <gtest/gtest.h>
#include <string>

#include "table.hpp"

using sstd::table;

TEST(TableIteratorCoverage, PostIncrement_ReturnsOldAndAdvances)
{
    table<int, int> t{{1, 10}, {2, 20}, {3, 30}};

    auto it = t.begin();
    ASSERT_NE(it, t.end());
    EXPECT_EQ(it->first, 1);

    auto old = it++;
    EXPECT_EQ(old->first, 1);
    ASSERT_NE(it, t.end());
    EXPECT_EQ(it->first, 2);
}

TEST(TableIteratorCoverage, PostDecrement_ReturnsOldAndMovesBack)
{
    table<int, int> t{{1, 10}, {2, 20}, {3, 30}};

    auto it = t.find(3);
    ASSERT_NE(it, t.end());
    EXPECT_EQ(it->first, 3);

    auto old = it--;
    EXPECT_EQ(old->first, 3);
    ASSERT_NE(it, t.end());
    EXPECT_EQ(it->first, 2);
}

TEST(TableIteratorCoverage, ConstIterator_DerefArrow_AndPostOps)
{
    table<int, std::string> t{{1, "one"}, {2, "two"}};
    const auto& ct = t;

    auto cit = ct.begin();
    ASSERT_NE(cit, ct.end());

    const auto& p = *cit;
    EXPECT_EQ(p.first, 1);
    EXPECT_EQ(cit->second, "one");

    auto old = cit++;
    EXPECT_EQ(old->first, 1);
    ASSERT_NE(cit, ct.end());
    EXPECT_EQ(cit->first, 2);

    auto old2 = cit--;
    EXPECT_EQ(old2->first, 2);
    ASSERT_NE(cit, ct.end());
    EXPECT_EQ(cit->first, 1);
}

TEST(TableIteratorCoverage, IteratorAndConstIterator_CompareAndConvert)
{
    table<int, int> t{{1, 10}, {2, 20}};

    auto it = t.begin();
    table<int,int>::const_iterator cit = it;

    EXPECT_TRUE(it == cit);
    EXPECT_FALSE(it != cit);
}

TEST(TableIteratorCoverage, o1)
{
    table<int, int> t{{1, 10}};

    auto it = t.end();
    auto& ref = ++it;

    EXPECT_EQ(it, t.end());
    EXPECT_EQ(&ref, &it);
}

TEST(TableIteratorCoverage, o2)
{
    table<int, int> t{{1, 10}};

    auto it = t.end();
    auto& ref = --it;

    EXPECT_EQ(it, t.end());
    EXPECT_EQ(&ref, &it);
}

TEST(TableIteratorCoverage, o3)
{
    table<int, int> t{{4, 40}, {2, 20}, {6, 60}, {1, 10}, {3, 30}};

    auto it = t.find(4);
    ASSERT_NE(it, t.end());

    --it;

    ASSERT_NE(it, t.end());
    EXPECT_EQ(it->first, 3);
    EXPECT_EQ(it->second, 30);
}

TEST(TableIteratorCoverage, p4)
{
    table<int, int> t{{4, 40}, {2, 20}, {6, 60}, {1, 10}, {3, 30}, {5, 50}, {7, 70}};

    auto it = t.begin();
    ASSERT_NE(it, t.end());
    EXPECT_EQ(it->first, 1);

    --it;

    EXPECT_EQ(it, t.end());
}


TEST(IteratorTest, iterator_test1) {
    table<int, std::string> t = {{1, "one"}, {2, "two"}, {3, "three"}};
    auto it = t.begin();
    EXPECT_NE(it, t.end());

    const auto& [key1, val1] = *it;
    EXPECT_EQ(key1, 1);
    EXPECT_EQ(val1, "one");

    EXPECT_EQ(it->first, 1);
    EXPECT_EQ(it->second, "one");

    it->second = "FIRST";
    EXPECT_EQ(t.at(1), "FIRST");
}

TEST(IteratorTest, iterator_test2) {
    table<int, int> tmp = {{10, 100}, {20, 200}};
    const table<int, int>& t = tmp;

    auto it = t.begin();
    EXPECT_NE(it, t.end());

    const auto& [key, val] = *it;
    EXPECT_EQ(key, 10);
    EXPECT_EQ(val, 100);

    EXPECT_EQ(it->first, 10);
    EXPECT_EQ(it->second, 100);
}

TEST(IteratorTest, iterator_test3) {
    table<int, int> t = {{1, 10}, {2, 20}};
    auto nonconst_it = t.begin();
    table<int, int>::const_iterator const_it = nonconst_it;

    EXPECT_NE(const_it, t.end());
    EXPECT_EQ(const_it->first, 1);
}

TEST(IteratorTest, iterator_test4) {
    table<int, int> t = {{3, 30}, {1, 10}, {2, 20}};
    std::vector<int> keys;

    for (auto it = t.begin(); it != t.end(); ++it) {
        keys.push_back(it->first);
    }

    EXPECT_EQ(keys, std::vector({1, 2, 3}));
}

TEST(IteratorTest, iterator_test5) {
    table<int, int> t = {{1, 10}, {2, 20}};

    auto it1 = t.find(1);
    auto it2 = t.begin();
    EXPECT_EQ(it1, it2);

    auto it3 = t.find(2);
    EXPECT_NE(it1, it3);
    EXPECT_EQ(it1, --it3);
    EXPECT_EQ(it1->first, it3--->first);

    EXPECT_EQ(t.begin(), t.begin());
    EXPECT_NE(t.begin(), t.end());
}

TEST(IteratorTest, iterator_test6) {
    table<int, int> t = {{1, 10}};
    auto nonconst_it = t.begin();
    const auto& ct = t;
    auto const_it = ct.begin();

    EXPECT_EQ(nonconst_it, const_it);
    EXPECT_EQ(const_it, nonconst_it);

    EXPECT_NE(nonconst_it, t.end());
    EXPECT_NE(const_it, t.cend());
}

TEST(IteratorTest, iterator_test7) {
    table<int, int> t = {{42, 420}};

    auto it = t.begin();
    EXPECT_NE(it, t.end());
    EXPECT_EQ(it->first, 42);

    it++;
    EXPECT_EQ(it, t.end());
}

TEST(IteratorTest, iterator_test8) {
    table<int, std::string> t = {{1, "one"}, {2, "two"}};
    std::vector<std::pair<const int, std::string>> result;

    for (const auto& p : t) {
        result.emplace_back(p);
    }

    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].first, 1);
    EXPECT_EQ(result[0].second, "one");
    EXPECT_EQ(result[1].first, 2);
    EXPECT_EQ(result[1].second, "two");
}
