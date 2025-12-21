#include "dataRace.hpp"

#include <thread>
#include <atomic>
#include <algorithm>
#include <execution>

size_t replace_stage_st(std::string& s, const std::string& from, const std::string& to) {
    if (from.empty()) return 0;

    size_t count = 0;
    std::string out;
    out.reserve(s.size());

    size_t pos = 0;
    while (true) {
        size_t found = s.find(from, pos);
        if (found == std::string::npos) break;

        out.append(s, pos, found - pos);
        out.append(to);
        pos = found + from.size();
        ++count;
    }

    out.append(s, pos, std::string::npos);
    s.swap(out);
    return count;
}

size_t replace_stage_mt(std::string& s, const std::string& from, const std::string& to,
                              size_t threads_quantity) {
    if (from.empty()) return 0;

    const size_t n = s.size();

    if (n < from.size()) return 0;

    const size_t fromLen = from.size();

    const unsigned hc = std::thread::hardware_concurrency();
    const size_t hw = hc ? static_cast<size_t>(hc) : 4;


    threads_quantity = std::max<size_t>(1, std::min(threads_quantity, hw));

    const size_t Up = n - fromLen;

    const size_t partSize = std::max<size_t>(4096, Up / (threads_quantity * 8) + 1);
    const size_t parts = (Up + partSize) / partSize;

    std::atomic_size_t nextPart{0};

    std::vector<std::vector<size_t>> local(threads_quantity);
    std::vector<std::thread> pool;
    pool.reserve(threads_quantity);

    auto worker = [&](const size_t tid) {
        auto& out = local[tid];
        while (true) {
            const size_t cid = nextPart.fetch_add(1, std::memory_order_relaxed);
            if (cid >= parts) break;

            const size_t begin = cid * partSize;
            const size_t end = std::min(Up + 1, begin + partSize);

            size_t pos = (begin >= (fromLen - 1)) ? (begin - (fromLen - 1)) : 0;

            while (true) {
                size_t found = s.find(from, pos);
                if (found == std::string::npos) break;
                if (found >= end) break;
                if (found >= begin) out.push_back(found);
                pos = found + 1;
            }
        }
    };

    for (size_t t = 0; t < threads_quantity; ++t) pool.emplace_back(worker, t);
    for (auto& th : pool) th.join();

    std::vector<size_t> starts;
    size_t total = 0;
    for (auto& v : local) total += v.size();
    starts.reserve(total);
    for (auto& v : local) starts.insert(starts.end(), v.begin(), v.end());
    std::sort( std::execution::par, starts.begin(), starts.end()); // Добавил многопоточку

    std::vector<size_t> keep;
    keep.reserve(starts.size());
    size_t lastEnd = 0;
    for (size_t st : starts) {
        if (st >= lastEnd) {
            keep.push_back(st);
            lastEnd = st + fromLen;
        }
    }

    std::string out;
    if (const ptrdiff_t delta = static_cast<ptrdiff_t>(to.size()) - static_cast<ptrdiff_t>(fromLen); delta > 0) out.reserve(s.size() + static_cast<size_t>(delta) * keep.size());
    else out.reserve(s.size());

    size_t prev = 0;
    for (const size_t st : keep) {
        out.append(s, prev, st - prev);
        out.append(to);
        prev = st + fromLen;
    }
    out.append(s, prev, std::string::npos);

    s.swap(out);
    return keep.size();
}



size_t replace(std::string& line, const std::vector<std::string>& v, int th) {
    if (v.empty() || (v.size() % 2 != 0)) return 0;

    size_t total = 0;
    for (size_t i = 0; i + 1 < v.size(); i += 2) {
        total += replace_stage_mt(line, v[i], v[i + 1], th);
    }
    return total;
}

size_t replaceOneThread(std::string& line, const std::vector<std::string>& v) {
    if (v.empty() || (v.size() % 2 != 0)) return 0;

    size_t total = 0;
    for (size_t i = 0; i + 1 < v.size(); i += 2) {
        total += replace_stage_st(line, v[i], v[i + 1]);
    }
    return total;
}