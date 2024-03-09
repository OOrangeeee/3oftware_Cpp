#include"tools.h"

bool dfs(int u, vector<vector<char>>& map, vector<pair<int, int>>& groupA, vector<pair<int, int>>& groupB, vector<bool>& visitedA, vector<int>& matchingB) {
	visitedA[u] = true;
	for (int v = 0; v < groupB.size(); v++) {
		if (matchingB[v] == -1) {
			vector<int> path = findShortestPath(map, groupA[u], groupB[v]);
			if (!path.empty()) {
				matchingB[v] = u;
				return true;
			}
		}
	}
	for (int v = 0; v < groupB.size(); v++) {
		if (matchingB[v] != -1 && !visitedA[matchingB[v]]) {
			vector<int> path = findShortestPath(map, groupA[u], groupB[v]);
			if (!path.empty() && dfs(matchingB[v], map, groupA, groupB, visitedA, matchingB)) {
				matchingB[v] = u;
				return true;
			}
		}
	}
	return false;
}

vector<pair<int, int>> findBijectiveMapping(vector<vector<char>>& map, vector<pair<int, int>>& groupA, vector<pair<int, int>>& groupB) {
	int n = groupA.size();
	vector<bool> visitedA(n, false);
	vector<int> matchingB(n, -1);
	vector<pair<int, int>> mapping;

	for (int u = 0; u < n; u++) {
		fill(visitedA.begin(), visitedA.end(), false);
		dfs(u, map, groupA, groupB, visitedA, matchingB);
	}

	for (int v = 0; v < n; v++) {
		if (matchingB[v] != -1) {
			mapping.push_back({ matchingB[v], v });
		}
	}

	return mapping;
}