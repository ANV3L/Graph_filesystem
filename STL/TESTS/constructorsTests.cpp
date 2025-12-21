#include "gtest/gtest.h"
#include "table.hpp"
#include <string>
#include <utility>

using sstd::table;

TEST(ConstructorsTest, DefaultConstructor1) {
    table<int, std::string> t;

    EXPECT_EQ(t.empty(), 1);
    EXPECT_EQ(t.size(), 0);
    EXPECT_EQ(t.begin(), t.end());
}

TEST(ConstructorsTest, DefaultConstructor2) {
    const table<int, int, std::greater<>> t;

    EXPECT_EQ(t.empty(), 1);
    EXPECT_EQ(t.size(), 0);
}

TEST(ConstructorsTest, CopyConstructor1) {
    const table<int, std::string> t1;
    table t2(t1);

    EXPECT_EQ(t2.empty(), 1);
    EXPECT_EQ(t2.size(), 0);
    EXPECT_EQ(t1.size(), t2.size());
}

TEST(ConstructorsTest, CopyConstructor2) {
    table<int, std::string> t1;
    t1.insert({1, "one"});

    table t2(t1);

    EXPECT_EQ(t2.size(), 1);
    EXPECT_FALSE(t2.empty());
    EXPECT_TRUE(t2.contains(1));
    EXPECT_EQ(t2.at(1), "one");

    t1.insert({2, "two"});
    EXPECT_EQ(t1.size(), 2);
    EXPECT_EQ(t2.size(), 1);
}

TEST(ConstructorsTest, CopyConstructor3) {
    table<int, std::string> t1;
    t1.insert({1, "one"});
    t1.insert({2, "two"});
    t1.insert({3, "three"});
    t1.insert({4, "four"});
    t1.insert({5, "five"});

    table t2(t1);

    EXPECT_EQ(t2.size(), 5);
    EXPECT_TRUE(t2.contains(1));
    EXPECT_TRUE(t2.contains(3));
    EXPECT_TRUE(t2.contains(5));
    EXPECT_EQ(t2.at(2), "two");
    EXPECT_EQ(t2.at(4), "four");
}

TEST(ConstructorsTest, CopyConstructor4) {
    table<int, std::string> t1;
    t1.insert({1, "one"});
    t1.insert({2, "two"});

    table t2(t1);

    t1.at(1) = "ONE";
    t1.erase(2);
    t1.insert({3, "three"});

    EXPECT_EQ(t2.at(1), "one");
    EXPECT_TRUE(t2.contains(2));
    EXPECT_FALSE(t2.contains(3));
    EXPECT_EQ(t2.size(), 2);
}

TEST(ConstructorsTest, CopyConstructor5) {
    table<int, int> t1;
    for (int i = 0; i < 1000; ++i) {
        t1.insert({i, i * 10});
    }

    table t2(t1);

    EXPECT_EQ(t2.size(), 1000);
    for (int i = 0; i < 1000; ++i) {
        EXPECT_TRUE(t2.contains(i));
        EXPECT_EQ(t2.at(i), i * 10);
    }
}

TEST(ConstructorsTest, CopyAssignment1) {
    table<int, std::string> t1;
    t1.insert({1, "one"});
    t1.insert({2, "two"});

    table<int, std::string> t2;
    t2.insert({10, "ten"});
    t2.insert({20, "twenty"});
    t2.insert({30, "thirty"});

    t2 = t1;

    EXPECT_EQ(t2.size(), 2);
    EXPECT_TRUE(t2.contains(1));
    EXPECT_TRUE(t2.contains(2));
    EXPECT_FALSE(t2.contains(10));
    EXPECT_FALSE(t2.contains(20));
}

TEST(ConstructorsTest, CopyAssignment2) {
    table<int, std::string> t1;
    t1.insert({1, "one"});
    t1.insert({2, "two"});

    t1 = t1;

    EXPECT_EQ(t1.size(), 2);
    EXPECT_TRUE(t1.contains(1));
    EXPECT_EQ(t1.at(2), "two");
}

TEST(ConstructorsTest, CopyAssignment3) {
    table<int, int> t1, t2;
    t1.insert({1, 10});

    table<int, int> t3 = t2 = t1;

    EXPECT_EQ(t2.size(), 1);
    EXPECT_EQ(t3.size(), 1);
    EXPECT_EQ(t3.at(1), 10);
}

TEST(ConstructorsTest, MoveConstructor1) {
    table<int, std::string> t1;
    table t2(std::move(t1));

    EXPECT_TRUE(t2.empty());
    EXPECT_EQ(t2.size(), 0);
}

TEST(ConstructorsTest, MoveConstructor2) {
    table<int, std::string> t1;
    t1.insert({1, "one"});
    t1.insert({2, "two"});
    t1.insert({3, "three"});

    table t2(std::move(t1));

    EXPECT_EQ(t2.size(), 3);
    EXPECT_TRUE(t2.contains(1));
    EXPECT_TRUE(t2.contains(2));
    EXPECT_TRUE(t2.contains(3));
    EXPECT_EQ(t2.at(1), "one");
}

TEST(ConstructorsTest, MoveAssignment1) {
    table<int, std::string> t1;
    table<int, std::string> t2;
    t2.insert({1, "temp"});

    t2 = std::move(t1);

    EXPECT_TRUE(t2.empty());
}

TEST(ConstructorsTest, MoveAssignment2) {
    table<int, std::string> t1;
    t1.insert({1, "one"});
    t1.insert({2, "two"});

    table<int, std::string> t2 = std::move(t1);

    EXPECT_EQ(t2.size(), 2);
    EXPECT_TRUE(t2.contains(1));
    EXPECT_EQ(t2.at(2), "two");
}

TEST(ConstructorsTest, MoveAssignment3) {
    table<int, std::string> t1;
    t1.insert({1, "one"});

    table<int, std::string> t2;
    t2.insert({10, "ten"});
    t2.insert({20, "twenty"});

    t2 = std::move(t1);

    EXPECT_EQ(t2.size(), 1);
    EXPECT_TRUE(t2.contains(1));
    EXPECT_FALSE(t2.contains(10));
}

TEST(ConstructorsTest, InitializerListConstructor1) {
    table<int, std::string> t = {{1, "one"}};

    EXPECT_EQ(t.size(), 1);
    EXPECT_TRUE(t.contains(1));
    EXPECT_EQ(t.at(1), "one");
}

TEST(ConstructorsTest, InitializerListConstructor2) {
    table<int, std::string> t = {
        {1, "one"},
        {2, "two"},
        {3, "three"},
        {4, "four"}
    };

    EXPECT_EQ(t.size(), 4);
    EXPECT_TRUE(t.contains(1));
    EXPECT_TRUE(t.contains(4));
    EXPECT_EQ(t.at(2), "two");
    EXPECT_EQ(t.at(3), "three");
}

TEST(ConstructorsTest, InitializerListConstructor3) {
    table<int, std::string> t = {
        {1, "one"},
        {2, "two"},
        {1, "ONE"},
        {3, "three"}
    };

    EXPECT_EQ(t.size(), 3);
    EXPECT_EQ(t.at(1), "one");
}

TEST(ConstructorsTest, InitializerListConstructor4) {
    table<int, int> t = {{5, 50}, {1, 10}, {3, 30}, {2, 20}, {4, 40}};

    std::vector<int> keys;
    for (const auto& [k, v] : t) {
        keys.push_back(k);
    }

    EXPECT_EQ(keys, std::vector<int>({1, 2, 3, 4, 5}));
}

TEST(ConstructorsTest, InitializerListConstructor5) {
    table<std::string, std::pair<int, int>> t = {
        {"first", {1, 10}},
        {"second", {2, 20}},
        {"third", {3, 30}}
    };

    EXPECT_EQ(t.size(), 3);
    EXPECT_EQ(t.at("first").first, 1);
    EXPECT_EQ(t.at("second").second, 20);
}

TEST(ConstructorsTest, InitializerList1) {
    table<int, std::string> t;
    t.insert({1, "temp"});

    t = {};

    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.size(), 0);
}

TEST(ConstructorsTest, InitializerList2) {
    table<int, std::string> t = {{1, "one"}, {2, "two"}};

    EXPECT_EQ(t.size(), 2);
    EXPECT_TRUE(t.contains(1));
    EXPECT_EQ(t.at(2), "two");
}

TEST(ConstructorsTest, InitializerList3) {
    table<int, std::string> t;
    t.insert({10, "ten"});
    t.insert({20, "twenty"});

    t = {{1, "one"}, {2, "two"}, {3, "three"}};

    EXPECT_EQ(t.size(), 3);
    EXPECT_FALSE(t.contains(10));
    EXPECT_FALSE(t.contains(20));
    EXPECT_TRUE(t.contains(1));
    EXPECT_TRUE(t.contains(3));
}

TEST(ConstructorsTest, InitializerList4) {
    table<int, int> t1;

    table<int, int> t2 = t1 = {{1, 10}, {2, 20}};

    EXPECT_EQ(t1.size(), 2);
    EXPECT_EQ(t2.size(), 2);
    EXPECT_EQ(t2.at(1), 10);
}

TEST(ConstructorsTest, InitializerList5) {
    table<int, std::string> t = {{1, "one"}};
    EXPECT_EQ(t.size(), 1);

    t = {{1, "ONE"}, {2, "TWO"}, {3, "THREE"}};
    EXPECT_EQ(t.size(), 3);
    EXPECT_EQ(t.at(1), "ONE");

    t = {};
    EXPECT_TRUE(t.empty());

    t = {{10, "ten"}};
    EXPECT_EQ(t.size(), 1);
}
