#include"tools.h"

int manhattanDistance(const pair<int, int>& a, const pair<int, int>& b)
{
	return abs(a.first - b.first) + abs(a.second - b.second);
}