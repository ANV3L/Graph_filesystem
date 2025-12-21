#include <gtest/gtest.h>

#include <string>
#include <vector>
#include <random>

#include "dataRace.hpp"

static void ExpectSameResult(const std::string& original,
                             const std::vector<std::string>& rules) {
    std::string mt = original;
    std::string st = original;

    const size_t c1 = ::replace(mt, rules);
    const size_t c2 = ::replaceOneThread(st, rules);

    EXPECT_EQ(mt, st) << "Result strings differ";
    EXPECT_EQ(c1, c2) << "Replacement counts differ";
}


TEST(THREADS, test1) {
    ExpectSameResult("hello world hello", {"hello", "hi"});
}

TEST(THREADS, test2) {
    ExpectSameResult("hello world", {"xyz", "abc"});
}

TEST(THREADS, test3) {
    std::string s1 = "abc";
    std::string s2 = "abc";
    std::vector<std::string> empty;

    EXPECT_EQ(::replace(s1, empty), 0u);
    EXPECT_EQ(::replaceOneThread(s2, empty), 0u);
    EXPECT_EQ(s1, "abc");
    EXPECT_EQ(s2, "abc");
}

TEST(THREADS, test4) {
    std::string s1 = "abc";
    std::string s2 = "abc";
    std::vector<std::string> bad = {"a"};

    EXPECT_EQ(::replace(s1, bad), 0u);
    EXPECT_EQ(::replaceOneThread(s2, bad), 0u);
    EXPECT_EQ(s1, "abc");
    EXPECT_EQ(s2, "abc");
}


TEST(THREADS, test5) {
    std::string s1 = "abc";
    std::string s2 = "abc";
    std::vector<std::string> r = {"", "X"};

    EXPECT_EQ(::replace(s1, r), 0u);
    EXPECT_EQ(::replaceOneThread(s2, r), 0u);
    EXPECT_EQ(s1, "abc");
    EXPECT_EQ(s2, "abc");
}

TEST(THREADS, test6) {
    ExpectSameResult("abc", {"abcd", "X"});
}

TEST(THREADS, test7) {
    ExpectSameResult("aaaa", {"aa", "X"});
}

TEST(THREADS, test8) {
    ExpectSameResult("aaaa", {"a", "aa"});
}

TEST(THREADS, test9) {
    ExpectSameResult("start middle end", {"start", "BEGIN", "BEGIN", "S", "end", "FIN"});
}


TEST(THREADS, test10) {
    std::string mt = "aa aa aa";
    std::string st = mt;
    std::vector<std::string> r = {"aa", "X"};

    const size_t c1 = ::replace(mt, r);
    const size_t c2 = ::replaceOneThread(st, r);

    ASSERT_EQ(mt, st);
    ASSERT_EQ(c1, c2);
    EXPECT_EQ(c1, 3u);
}


TEST(THREADS, test11) {
    std::mt19937 rng(123456);

    std::uniform_int_distribution<int> lenDist(0, 200);
    std::uniform_int_distribution<int> charDist(0, 4);
    std::uniform_int_distribution<int> rulesDist(1, 4);
    std::uniform_int_distribution<int> fromLenDist(1, 4);
    std::uniform_int_distribution<int> toLenDist(0, 6);

    auto randStr = [&](int n) {
        std::string s;
        s.reserve(static_cast<size_t>(n));
        for (int i = 0; i < n; ++i) s.push_back(char('a' + charDist(rng)));
        return s;
    };

    for (int tc = 0; tc < 300; ++tc) {
        const std::string text = randStr(lenDist(rng));

        const int rulesN = rulesDist(rng);
        std::vector<std::string> rules;
        rules.reserve((size_t)rulesN * 2);

        for (int i = 0; i < rulesN; ++i) {
            rules.push_back(randStr(fromLenDist(rng)));
            rules.push_back(randStr(toLenDist(rng)));
        }

        std::string mt = text;
        std::string st = text;

        const size_t c1 = ::replace(mt, rules);
        const size_t c2 = ::replaceOneThread(st, rules);

        ASSERT_EQ(mt, st) << "Mismatch on tc=" << tc;
        ASSERT_EQ(c1, c2) << "Count mismatch on tc=" << tc;
    }
}
