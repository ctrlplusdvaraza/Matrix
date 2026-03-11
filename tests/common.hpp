#pragma once

#include <random>

inline int GenerateRandomInt(int from, int to) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(from, to);

    return distrib(gen);
}
