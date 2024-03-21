#include"tools.h"

int manhattanDistance(const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
    return std::abs(p1.first - p2.first) + std::abs(p1.second - p2.second);
}