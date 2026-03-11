#pragma once

#include <random>
#include <vector>

inline std::vector<int> GenerateRandomInts(std::size_t n, int from, int to) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(from, to);

    std::vector<int> data;
    data.reserve(n);

    for (std::size_t i = 0; i < n; ++i) {
        data.push_back(distrib(gen));
    }

    return data;
}
