#include "gtest/gtest.h"
#include "table.hpp"
#include <string>
#include <vector>
#include <random>

using sstd::table;

constexpr unsigned int GLOBAL_SEED = 42;

TEST(ModifiersTest, Clear) {
    table<int, int> t;
    t.clear();
    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.size(), 0);
}

TEST(ModifiersTest, Clear2) {
    table<int, std::string> t = {{1, "one"}, {2, "two"}};
    t.clear();
    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.size(), 0);
    EXPECT_FALSE(t.contains(1));
    EXPECT_FALSE(t.contains(2));
}

TEST(ModifiersTest, Insert1) {
    table<int, int> t;
    auto [it, inserted] = t.insert({5, 50});
    EXPECT_TRUE(inserted);
    EXPECT_EQ(t.size(), 1);
    EXPECT_EQ(it->first, 5);
    EXPECT_EQ(it->second, 50);
}

TEST(ModifiersTest, Insert2) {
    table<int, int> t = {{1, 10}};
    auto [it, inserted] = t.insert({1, 100});
    EXPECT_FALSE(inserted);
    EXPECT_EQ(t.size(), 1);
    EXPECT_EQ(t.at(1), 10);
}

TEST(ModifiersTest, Insert3) {
    table<int, std::string> t;
    t.insert({1, "one"});
    t.insert({3, "three"});
    t.insert({2, "two"});
    EXPECT_EQ(t.size(), 3);
    EXPECT_EQ(t.at(1), "one");
    EXPECT_EQ(t.at(2), "two");
    EXPECT_EQ(t.at(3), "three");
}

TEST(ModifiersTest, Insert4) {
    table<int, int> t;
    t.insert({{1, 10}, {2, 20}, {1, 100}});
    EXPECT_EQ(t.size(), 2);
    EXPECT_EQ(t.at(1), 10);
    EXPECT_EQ(t.at(2), 20);
}

TEST(TableInsertRangeTest, insert5) {
    table<int, std::string> t;

    std::vector<std::pair<int, std::string>> data = {
        {1, "one"}, {2, "two"}, {3, "three"}
    };

    t.insert(data.begin(), data.end());

    EXPECT_EQ(t.size(), 3);
    EXPECT_EQ(t.at(1), "one");
    EXPECT_EQ(t.at(2), "two");
    EXPECT_EQ(t.at(3), "three");
}

TEST(TableInsertRangeTest, insert6) {
    table<int, std::string> t;
    t.insert({1, "one"});

    std::vector<std::pair<int, std::string>> new_data = {
        {2, "two"}, {3, "three"}
    };

    t.insert(new_data.begin(), new_data.end());

    EXPECT_EQ(t.size(), 3);
    EXPECT_EQ(t.at(1), "one");
    EXPECT_EQ(t.at(2), "two");
}

TEST(TableInsertRangeTest, insert7) {
    table<int, std::string> t;
    std::map<int, std::string> source_map = {
        {10, "ten"}, {20, "twenty"}
    };

    t.insert(source_map.begin(), source_map.end());

    EXPECT_EQ(t.size(), 2);
    EXPECT_EQ(t.at(10), "ten");
}



TEST(TableAssignRangeTest, assign1) {
    table<int, std::string> t;
    t.insert({100, "old_data"});

    std::vector<std::pair<int, std::string>> new_data = {
        {1, "one"}, {2, "two"}
    };

    t.assign(new_data.begin(), new_data.end());

    EXPECT_EQ(t.size(), 2);
    EXPECT_EQ(t.at(1), "one");
    EXPECT_EQ(t.at(2), "two");

    EXPECT_FALSE(t.contains(100));
}

TEST(TableAssignRangeTest, assign2) {
    table<int, std::string> t;
    t.insert({1, "one"});
    t.insert({2, "two"});

    std::vector<std::pair<int, std::string>> empty_vec;

    t.assign(empty_vec.begin(), empty_vec.end());

    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.size(), 0);
}

TEST(TableAssignRangeTest, assign3) {
    table<int, std::string> t;
    std::pair<int, std::string> arr[] = {
        {5, "five"}, {6, "six"}
    };

    t.assign(std::begin(arr), std::end(arr));

    EXPECT_EQ(t.size(), 2);
    EXPECT_EQ(t.at(5), "five");
}

TEST(TableAssignInitListTest, assign4) {
    table<int, std::string> t;
    t.insert({999, "trash"});

    t.assign({{1, "one"},{2, "two"},{3, "three"}});

    EXPECT_EQ(t.size(), 3);
    EXPECT_EQ(t.at(1), "one");
    EXPECT_FALSE(t.contains(999));
}

TEST(TableAssignInitListTest, assign5) {
    table<int, std::string> t;
    t.insert({1, "val"});

    t.assign({});

    EXPECT_TRUE(t.empty());
}

TEST(TableAssignInitListTest, assign6) {
    table<int, std::string> t;
    t.assign({{42, "answer"}});

    EXPECT_EQ(t.size(), 1);
    EXPECT_EQ(t.at(42), "answer");
}

TEST(TableAssignTest, assign7) {
    table<int, std::string> t;
    t.insert({1, "original"});

    std::vector<std::pair<int, std::string>> data = {{2, "new"}};

    t.assign(data.begin(), data.end());

    EXPECT_EQ(t.size(), 1);
    EXPECT_EQ(t.at(2), "new");
}

TEST(ModifiersTest, InsertOrAssign1) {
    table<int, int> t;
    auto [it, inserted] = t.insert_or_assign({5, 50});
    EXPECT_TRUE(inserted);
    EXPECT_EQ(t.size(), 1);
    EXPECT_EQ(t.at(5), 50);
}

TEST(ModifiersTest, InsertOrAssign2) {
    table<int, int> t = {{1, 10}};
    auto [it, inserted] = t.insert_or_assign({1, 100});
    EXPECT_FALSE(inserted);
    EXPECT_EQ(t.size(), 1);
    EXPECT_EQ(t.at(1), 100);
}

TEST(ModifiersTest, InsertOrAssign3) {
    table<int, int> t = {{1, 10}};
    auto [it, inserted] = t.insert_or_assign({1, 100});
    EXPECT_FALSE(inserted);
    EXPECT_EQ(t.size(), 1);
    EXPECT_EQ(t.at(1), 100);
}

TEST(ModifiersTest, Emplace1) {
    table<int, std::string> t;
    auto [it, inserted] = t.emplace(42, "forty-two");
    EXPECT_TRUE(inserted);
    EXPECT_EQ(t.size(), 1);
    EXPECT_EQ(t.at(42), "forty-two");
}

TEST(ModifiersTest, Emplace2) {
    table<int, int> t = {{1, 10}};
    auto [it, inserted] = t.emplace(1, 100);
    EXPECT_FALSE(inserted);
    EXPECT_EQ(t.at(1), 10);
}

TEST(ModifiersTest, Emplace3) {
    table<int, int> t = {{1, 10}, {3, 30}, {5, 50}};
    auto it = t.find(3);
    auto hint_it = t.emplace_hint(it, 4, 40);
    EXPECT_EQ(hint_it->first, 4);
    EXPECT_EQ(t.size(), 4);
}

TEST(ModifiersTest, TryEmplace1) {
    table<int, std::string> t;
    auto [it, inserted] = t.try_emplace(100, "hundred");
    EXPECT_TRUE(inserted);
    EXPECT_EQ(t.size(), 1);
    EXPECT_EQ(t.at(100), "hundred");
}

TEST(ModifiersTest, TryEmplace2) {
    table<int, int> t = {{1, 10}};
    auto [it, inserted] = t.try_emplace(1, 100);
    EXPECT_FALSE(inserted);
    EXPECT_EQ(t.at(1), 10);
}

TEST(ModifiersTest, Erase1) {
    table<int, int> t = {{1, 10}, {2, 20}, {3, 30}};
    auto it = t.find(2);
    auto next_it = t.erase(it);
    EXPECT_EQ(t.size(), 2);
    EXPECT_FALSE(t.contains(2));
    EXPECT_EQ(next_it->first, 3);
}

TEST(ModifiersTest, Erase2) {
    table<int, int> t = {{1, 10}};
    EXPECT_EQ(t.erase(t.end()), t.end());
}

TEST(ModifiersTest, Erase3) {
    table<int, int> t = {{1, 10}, {2, 20}};
    const auto& ct = t;
    auto cit = ct.find(1);
    auto next_it = t.erase(cit);
    EXPECT_EQ(next_it->second, 20);
    EXPECT_EQ(t.size(), 1);
    EXPECT_FALSE(t.contains(1));
}

TEST(ModifiersTest, Erase4) {
    table<int, int> t = {{1, 10}, {2, 20}, {3, 30}, {4, 40}};
    auto first = t.find(2);
    auto last = t.find(4);
    auto it = t.erase(first, last);
    EXPECT_EQ(t.size(), 2);
    EXPECT_TRUE(t.contains(1));
    EXPECT_FALSE(t.contains(2));
    EXPECT_EQ(it->first, 4);
}

TEST(ModifiersTest, Erase5) {
    table<int, int> t = {{1, 10}, {2, 20}};
    EXPECT_EQ(t.erase(1), 1);
    EXPECT_EQ(t.size(), 1);
    EXPECT_FALSE(t.contains(1));
}

TEST(ModifiersTest, Erase6) {
    table<int, int> t = {{1, 10}};
    EXPECT_EQ(t.erase(999), 0);
    EXPECT_EQ(t.size(), 1);
}

TEST(ModifiersTest, Swap) {
    table<int, int> t1 = {{1, 10}, {2, 20}};
    table<int, int> t2 = {{100, 1000}};
    swap(t1, t2);
    EXPECT_EQ(t1.size(), 1);
    EXPECT_EQ(t1.at(100), 1000);
    EXPECT_EQ(t2.size(), 2);
    EXPECT_EQ(t2.at(1), 10);
}

TEST(ModifiersTest, Swap2) {
    table<int, int> t1, t2 = {{1, 10}};
    swap(t1, t2);
    EXPECT_TRUE(t1.contains(1));
    EXPECT_TRUE(t2.empty());
}

TEST(ModifiersTest, Swap3) {
    table<int, int> t = {{1, 10}};
    swap(t, t);
    EXPECT_EQ(t.size(), 1);
    EXPECT_EQ(t.at(1), 10);
}

TEST(TableStress, extra1) {
    table<int, int> t;
    const int N = 100;

    for (int i = 1; i <= N; i++) {
        auto [it, inserted] = t.insert({i, i * 10});
        EXPECT_TRUE(inserted) << "Элемент " << i << " не вставился";
        EXPECT_EQ(it->first, i);
        EXPECT_EQ(it->second, i * 10);
    }

    EXPECT_EQ(t.size(), N) << "Размер таблицы должен быть " << N;

    for (int i = 1; i <= N; i++) {
        EXPECT_TRUE(t.contains(i)) << "Элемент " << i << " потерялся при вставке";
        auto it = t.find(i);
        EXPECT_NE(it, t.end());
        EXPECT_EQ(it->second, i * 10) << "Значение элемента " << i << " неправильное";
    }

    std::vector<int> keys;
    for (auto& [k, v] : t) {
        keys.push_back(k);
    }
    EXPECT_EQ(keys.size(), N);
    for (int i = 0; i < N; i++) {
        EXPECT_EQ(keys[i], i + 1) << "Порядок нарушен на позиции " << i;
    }
}

TEST(TableStress, extra2) {
    table<int, std::string> t;
    const int N = 100;

    for (int i = N; i >= 1; i--) {
        auto [it, inserted] = t.insert({i, "val_" + std::to_string(i)});
        EXPECT_TRUE(inserted) << "Элемент " << i << " не вставился в обратном порядке";
    }

    EXPECT_EQ(t.size(), N);

    for (int i = 1; i <= N; i++) {
        EXPECT_TRUE(t.contains(i)) << "Элемент " << i << " потерялся при обратной вставке";
    }

    int prev = 0;
    int count = 0;
    for (auto& [k, v] : t) {
        EXPECT_GT(k, prev) << "Порядок нарушен: " << k << " <= " << prev;
        prev = k;
        count++;
    }
    EXPECT_EQ(count, N) << "Итерировали неправильное количество элементов";
}

TEST(TableStress, extra3) {
    table<int, int> t;
    std::vector keys = {
        25, 10, 75, 5, 15, 30, 80, 2, 8, 12, 18, 22, 28, 35, 40,
        50, 60, 70, 85, 90, 95, 3, 7, 20, 45, 55, 65, 88, 92, 98,
        1, 4, 6, 9, 11, 13, 16, 19, 23, 26, 29, 32, 38, 42, 48,
        52, 58, 62, 68, 72, 78
    };

    const int N = static_cast<int>(keys.size());
    ASSERT_GE(N, 40) << "Нужно минимум 40 элементов";

    for (int key : keys) {
        auto [it, inserted] = t.insert({key, key * 100});
        EXPECT_TRUE(inserted) << "Элемент " << key << " не вставился при случайной вставке";
        EXPECT_EQ(it->first, key);
        EXPECT_EQ(it->second, key * 100);
    }

    EXPECT_EQ(t.size(), N);

    for (int key : keys) {
        EXPECT_TRUE(t.contains(key)) << "Элемент " << key << " потерялся";
        auto it = t.find(key);
        EXPECT_NE(it, t.end());
        EXPECT_EQ(it->second, key * 100);
    }

    std::vector<int> iterated_keys;
    for (auto& [k, v] : t) {
        iterated_keys.push_back(k);
    }
    EXPECT_EQ(iterated_keys.size(), N);

    for (size_t i = 1; i < iterated_keys.size(); i++) {
        EXPECT_LT(iterated_keys[i-1], iterated_keys[i])
            << "Порядок нарушен: " << iterated_keys[i-1] << " >= " << iterated_keys[i];
    }
}

TEST(TableStress, extra4) {
    table<int, int> t;

    for (int i = 1; i <= 30; i++) {
        auto [it1, ins1] = t.insert({i, i});
        EXPECT_TRUE(ins1) << "Маленькое число " << i << " не вставилось";

        int large = 101 - i;
        auto [it2, ins2] = t.insert({large, large});
        EXPECT_TRUE(ins2) << "Большое число " << large << " не вставилось";
    }

    EXPECT_EQ(t.size(), 60);

    for (int i = 1; i <= 30; ++i) {
        EXPECT_TRUE(t.contains(i)) << "Элемент " << i << " потерялся";
        int large = 101 - i;
        EXPECT_TRUE(t.contains(large)) << "Элемент " << large << " потерялся";
    }


    int prev = 0;
    int count = 0;
    for (auto& [k, v] : t) {
        EXPECT_GT(k, prev) << "Порядок нарушен при чередующейся вставке";
        prev = k;
        count++;
    }
    EXPECT_EQ(count, 60);
}

TEST(TableStress, extra5) {
    table<int, std::string> t;

    for (int i = 1; i <= 50; i++) {
        auto [it, inserted] = t.insert({i, "original_" + std::to_string(i)});
        EXPECT_TRUE(inserted) << "Первая вставка элемента " << i << " не удалась";
        EXPECT_EQ(it->second, "original_" + std::to_string(i));
    }

    EXPECT_EQ(t.size(), 50);

    for (int i = 1; i <= 50; i++) {
        auto [it, inserted] = t.insert({i, "duplicate_" + std::to_string(i)});
        EXPECT_FALSE(inserted) << "Дублирующийся элемент " << i << " не должен был вставиться";
        EXPECT_EQ(it->second, "original_" + std::to_string(i))
            << "Значение элемента " << i << " неправильно изменилось";
    }

    EXPECT_EQ(t.size(), 50) << "Размер изменился при попытке вставить дубликаты";

    for (int i = 1; i <= 50; i++) {
        EXPECT_TRUE(t.contains(i));
        EXPECT_EQ(t.find(i)->second, "original_" + std::to_string(i));
    }
}

TEST(TableStress, extra6) {
    table<int, int> t;
    const int N = 1000;

    std::vector<int> sequence;
    for (int i = 0; i < N; i++) {
        sequence.push_back(i);
    }

    std::mt19937 rng(GLOBAL_SEED);
    std::shuffle(sequence.begin(), sequence.end(), rng);


    for (int i = 0; i < N; i++) {
        int key = sequence[i];
        auto [it, inserted] = t.insert({key, key * key});
        EXPECT_TRUE(inserted) << "Элемент " << key << " не вставился при большой таблице";
    }

    EXPECT_EQ(t.size(), N);

    for (int i = 0; i < N; i += 10) {
        EXPECT_TRUE(t.contains(i)) << "Элемент " << i << " потерялся в большой таблице";
    }

    int prev = -1;
    int count = 0;
    for (auto& [k, v] : t) {
        EXPECT_GT(k, prev) << "Порядок нарушен при большой таблице";
        prev = k;
        count++;
    }
    EXPECT_EQ(count, N) << "Итерировали неправильное количество элементов в большой таблице";
}

TEST(TableStress, extra7) {
    table<std::string, int> t;

    std::vector<std::string> keys = {
        "alpha", "beta", "gamma", "delta", "epsilon",
        "zeta", "eta", "theta", "iota", "kappa",
        "lambda", "mu", "nu", "xi", "omicron",
        "pi", "rho", "sigma", "tau", "upsilon",
        "phi", "chi", "psi", "omega", "apple",
        "banana", "cherry", "date", "fig", "grape",
        "hazelnut", "ice", "jackfruit", "kiwi", "lemon",
        "mango", "nectarine", "orange", "papaya", "quince",
        "raspberry", "strawberry", "tomato", "ugli", "vanilla",
        "watermelon", "xigua", "yam", "zucchini", "avocado"
    };

    const std::size_t N = keys.size();
    ASSERT_GE(N, 40u);

    std::mt19937 rng(GLOBAL_SEED);
    std::shuffle(keys.begin(), keys.end(), rng);

    for (std::size_t i = 0; i < N; ++i) {
        auto [it, inserted] = t.insert({keys[i], static_cast<int>(i)});
        EXPECT_TRUE(inserted) << "Ключ " << keys[i] << " не вставился";
        EXPECT_EQ(it->first, keys[i]);
    }

    EXPECT_EQ(t.size(), N);

    for (std::size_t i = 0; i < N; ++i) {
        const auto &k = keys[i];
        EXPECT_TRUE(t.contains(k)) << "Ключ " << k << " потерялся";
        auto it = t.find(k);
        EXPECT_NE(it, t.end());
    }

    std::vector<std::string> iter_keys;
    iter_keys.reserve(N);
    for (auto &[k, v] : t) {
        iter_keys.push_back(k);
    }
    EXPECT_EQ(iter_keys.size(), N);

    for (std::size_t i = 1; i < iter_keys.size(); ++i) {
        EXPECT_LT(iter_keys[i - 1], iter_keys[i])
            << "Порядок строковых ключей нарушен: "
            << iter_keys[i - 1] << " >= " << iter_keys[i];
    }
}


#include <set>
#include <algorithm>

using Table = table<std::string, int>;

static bool check_bst_property(const Table &t) {
    std::string prev;
    bool first = true;
    for (const auto &[k, v] : t) {
        if (!first && !(prev < k)) return false;
        prev = k;
        first = false;
    }
    return true;
}

TEST(EraseStress, extra8) {
    Table t;
    const int n = 2000;

    for (int i = 0; i < n; ++i) {
        t.insert({std::to_string(i), i});
    }
    ASSERT_EQ(t.size(), static_cast<Table::size_type>(n));

    std::vector<int> order(n);
    for (int i = 0; i < n; ++i) order[i] = i;

    std::mt19937 rng(GLOBAL_SEED);
    std::shuffle(order.begin(), order.end(), rng);

    std::set<std::string> alive;
    for (int i = 0; i < n; ++i) alive.insert(std::to_string(i));

    for (int x : order) {
        std::string k = std::to_string(x);

        ASSERT_TRUE(t.contains(k));
        ASSERT_TRUE(alive.count(k) == 1);

        auto erased = t.erase(k);
        ASSERT_EQ(erased, 1u) << "erase(key) должен вернуть 1 для существующего ключа";

        alive.erase(k);
        ASSERT_FALSE(t.contains(k));

        ASSERT_EQ(t.size(), alive.size());

        ASSERT_TRUE(check_bst_property(t));
    }

    ASSERT_TRUE(t.empty());
}


TEST(EraseStress, extra9) {
    Table t;
    const int n = 1000;

    for (int i = 0; i < n; ++i) {
        t.insert({std::to_string(i), i});
    }
    ASSERT_EQ(t.size(), static_cast<Table::size_type>(n));

    std::set<std::string> alive;
    for (int i = 0; i < n; ++i) alive.insert(std::to_string(i));

    std::mt19937 rng(GLOBAL_SEED);

    while (!t.empty()) {
        std::uniform_int_distribution<> dist(0, static_cast<int>(t.size()) - 1);
        int steps = dist(rng);

        auto it = t.begin();
        std::advance(it, steps);
        std::string key = it->first;

        auto next = t.erase(it);

        alive.erase(key);
        ASSERT_EQ(t.size(), alive.size());

        if (next != t.end()) {
            ASSERT_TRUE(alive.count(next->first) == 1);
        }

        ASSERT_TRUE(check_bst_property(t));
    }

    ASSERT_TRUE(t.empty());
}

TEST(EraseStress, extra10) {
    Table t;
    const int n = 300;

    for (int i = 0; i < n; ++i) {
        t.insert({std::to_string(i), i});
    }
    ASSERT_EQ(t.size(), static_cast<Table::size_type>(n));

    std::set<std::string> alive;
    for (int i = 0; i < n; ++i) alive.insert(std::to_string(i));

    std::mt19937 rng(GLOBAL_SEED);

    for (int iter = 0; iter < 100 && !t.empty(); ++iter) {
        if (t.size() <= 1) break;

        int sz = static_cast<int>(t.size());
        std::uniform_int_distribution<> dpos(0, sz - 1);
        int a = dpos(rng);
        int b = dpos(rng);
        if (a > b) std::swap(a, b);
        if (a == b) {
            if (b + 1 < sz) ++b;
            else if (a > 0) --a;
        }

        auto first = t.begin();
        std::advance(first, a);

        auto last = t.begin();
        std::advance(last, b);

        std::vector<std::string> to_erase;
        for (auto it = first; it != last; ++it) {
            to_erase.push_back(it->first);
        }

        auto it_after = t.erase(first, last);

        for (auto &k : to_erase) {
            alive.erase(k);
            ASSERT_FALSE(t.contains(k));
        }

        ASSERT_EQ(t.size(), alive.size());
        ASSERT_TRUE(check_bst_property(t));

        if (it_after != t.end()) {
            ASSERT_TRUE(alive.count(it_after->first) == 1);
        }
    }
}

TEST(EraseStress, extra11) {
    Table t;
    const int n = 400;

    for (int i = 0; i < n; ++i) {
        t.insert({std::to_string(i), i});
    }

    std::set<std::string> alive;
    for (int i = 0; i < n; ++i) alive.insert(std::to_string(i));

    std::mt19937 rng(GLOBAL_SEED);
    std::uniform_int_distribution<> op_dist(0, 2);

    while (!t.empty()) {
        int op = op_dist(rng);

        if (op == 0) {
            int x = static_cast<int>(t.size()) / 2;
            std::string k = std::to_string(x);
            if (alive.count(k)) {
                auto erased = t.erase(k);
                ASSERT_EQ(erased, 1u);
                alive.erase(k);
            }
        } else if (op == 1 && !t.empty()) {
            auto it = t.begin();
            std::advance(it, t.size() / 3);
            std::string k = it->first;
            t.erase(it);
            alive.erase(k);
        } else if (op == 2 && t.size() > 2) {

            int sz = static_cast<int>(t.size());
            int a = sz / 4;
            int b = sz / 2;
            auto first = t.begin();
            std::advance(first, a);
            auto last = t.begin();
            std::advance(last, b);

            std::vector<std::string> keys;
            for (auto it = first; it != last; ++it) {
                keys.push_back(it->first);
            }
            t.erase(first, last);
            for (auto &k : keys) alive.erase(k);
        }

        ASSERT_EQ(t.size(), alive.size());
        ASSERT_TRUE(check_bst_property(t));
    }

    ASSERT_TRUE(t.empty());
}

TEST(StressInsertErase, Stress_insert2) {
    table<int, int> t;
    std::map<int,int> ref;

    std::mt19937 rng(GLOBAL_SEED);
    std::uniform_int_distribution<int> key_dist(0, 50000);
    std::uniform_int_distribution<int> val_dist(0, 1000000);

    const int N = 100000;

    {
        std::vector<table<int, int>::value_type> batch;
        batch.reserve(1000);
        for (int i = 0; i < 1000; ++i) {
            int k = key_dist(rng);
            int v = val_dist(rng);
            batch.emplace_back(k, v);
            ref.insert({k, v});
        }
        t.insert(batch.begin(), batch.end());

        t.insert({ { -1, 111 }, { -2, 222 }, { -3, 333 }, { -1, 999 } });
        ref.insert({-1,111});
        ref.insert({-2,222});
        ref.insert({-3,333});
    }

    for (int i = 0; i < N; ++i) {
        int op = std::uniform_int_distribution<int>(0, 6)(rng);
        int k = key_dist(rng);
        int v = val_dist(rng);

        switch (op) {
            case 0: {
                table<int, int>::value_type val{k, v};
                auto [it, ins] = t.insert(val);
                auto [rit, rins] = ref.insert({k, v});
                ASSERT_EQ(rins, ins);
                if (ins) {
                    ASSERT_EQ(rit->first, it->first);
                    ASSERT_EQ(rit->second, it->second);
                }
                break;
            }
            case 1: {
                auto [it, ins] = t.insert(table<int, int>::value_type{k, v});
                auto [rit, rins] = ref.insert({k, v});
                ASSERT_EQ(rins, ins);
                if (ins) {
                    ASSERT_EQ(rit->first, it->first);
                    ASSERT_EQ(rit->second, it->second);
                }
                break;
            }
            case 2: {
                table<int, int>::value_type val{k, v};
                auto [it, ins] = t.insert_or_assign(val);
                auto [rit, rins] = ref.insert({k, v});
                if (!rins) rit->second = v;
                ASSERT_EQ(ref.at(k), it->second);
                break;
            }
            case 3: {
                auto [it, ins] = t.insert_or_assign(table<int, int>::value_type{k, v});
                auto [rit, rins] = ref.insert({k, v});
                if (!rins) rit->second = v;
                ASSERT_EQ(ref.at(k), it->second);
                break;
            }
            case 4: {
                auto [it, ins] = t.emplace(k, v);
                auto [rit, rins] = ref.insert({k, v});
                ASSERT_EQ(rins, ins);
                ASSERT_EQ(rit->second, it->second);
                break;
            }
            case 5: {
                auto hint = (t.empty() ? t.end() : t.begin());
                auto it = t.emplace_hint(hint, k, v);
                auto [rit, rins] = ref.insert({k, v});
                ASSERT_EQ(ref.at(k), it->second);
                break;
            }
            case 6: {
                if (std::uniform_int_distribution<int>(0,1)(rng) == 0) {
                    auto [it, ins] = t.try_emplace(k, v);
                    auto [rit, rins] = ref.insert({k, v});
                    if (!rins) { /* value не меняется */ }
                    ASSERT_EQ(ref.at(k), it->second);
                } else {
                    auto [it, ins] = t.try_emplace(static_cast<int>(k), v);
                    auto [rit, rins] = ref.insert({k, v});
                    ASSERT_EQ(ref.at(it->first), it->second);
                }
                break;
            }
            default: break;
        }

        if (i % 1000 == 0) {
            ASSERT_EQ(ref.size(), t.size());
        }
    }

    ASSERT_EQ(ref.size(), t.size());
    auto tit = t.begin();
    auto rit = ref.begin();
    for (; rit != ref.end(); ++rit, ++tit) {
        ASSERT_EQ(rit->first, tit->first);
        ASSERT_EQ(rit->second, tit->second);
    }
}

TEST(StressInsertErase, Stress_erase2) {
    using Table = table<int, int>;
    Table t;
    std::map<int,int> ref;
    std::mt19937 rng(GLOBAL_SEED);
    std::uniform_int_distribution<int> key_dist(0, 50000);

    const int N = 100000;
    for (int i = 0; i < N; ++i) {
        int k = key_dist(rng);
        int v = i;
        t.insert({k, v});
        ref.insert({k, v});
    }

    ASSERT_EQ(ref.size(), t.size());

    auto check_consistency = [&] {
        ASSERT_EQ(ref.size(), t.size());
        auto tit = t.begin();
        auto rit = ref.begin();
        for (; rit != ref.end(); ++rit, ++tit) {
            ASSERT_EQ(rit->first, tit->first);
            ASSERT_EQ(rit->second, tit->second);
        }
    };

    {
        std::vector<int> keys;
        keys.reserve(ref.size());
        for (auto &[k,v] : ref) keys.push_back(k);

        std::mt19937 rng(GLOBAL_SEED);
        std::shuffle(keys.begin(), keys.end(), rng);


        for (int k : keys) {
            size_t e1 = t.erase(k);
            size_t e2 = ref.erase(k);
            ASSERT_EQ(e2, e1);
            ASSERT_EQ(ref.size(), t.size());
        }
        ASSERT_TRUE(t.empty());
        ASSERT_TRUE(ref.empty());
    }

    for (int i = 0; i < N; ++i) {
        int k = key_dist(rng);
        int v = i;
        t.insert({k, v});
        ref.insert({k, v});
    }

    while (!t.empty()) {
        int sz = static_cast<int>(t.size());
        std::uniform_int_distribution<int> op_dist(0, 1);
        int op = op_dist(rng);

        if (op == 0) {
            int offset = std::uniform_int_distribution<int>(0, sz-1)(rng);
            auto it = t.begin();
            std::advance(it, offset);
            int k = it->first;

            t.erase(it);
            ref.erase(k);
        } else {
            int offset = std::uniform_int_distribution<int>(0, sz-1)(rng);
            Table::const_iterator cit = t.cbegin();
            std::advance(cit, offset);
            int k = cit->first;

            t.erase(cit);
            ref.erase(k);
        }

        if (t.size() % 1000 == 0) {
            check_consistency();
        }
    }
    ASSERT_TRUE(ref.empty());
    ASSERT_TRUE(t.empty());

    for (int i = 0; i < N; ++i) {
        int k = key_dist(rng);
        int v = i;
        t.insert({k, v});
        ref.insert({k, v});
    }

    while (t.size() > 0) {
        int sz = static_cast<int>(t.size());
        if (sz < 2) break;
        int a = std::uniform_int_distribution<int>(0, sz-1)(rng);
        int b = std::uniform_int_distribution<int>(0, sz-1)(rng);
        if (a > b) std::swap(a, b);
        if (a == b) {
            if (b+1 < sz) ++b;
            else if (a > 0) --a;
        }

        auto first = t.begin();
        std::advance(first, a);
        auto last = t.begin();
        std::advance(last, b);

        std::vector<int> to_erase;
        for (auto it = first; it != last; ++it) {
            to_erase.push_back(it->first);
        }

        t.erase(first, last);
        for (int k : to_erase) {
            ref.erase(k);
        }

        check_consistency();
    }
}

