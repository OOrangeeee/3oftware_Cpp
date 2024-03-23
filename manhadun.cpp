#include"tools.h"

int manhattanDistance(const pair<int, int>& p1, const pair<int, int>& p2)
{
	return abs(p1.first - p2.first) + abs(p1.second - p2.second);
}