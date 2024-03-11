#include"tools.h"
bool isReachable(const vector<vector<char>>& map, pair<int, int> start, pair<int, int> end)
{
	if (map[start.first][start.second] == '*' || map[end.first][end.second] == '*' || map[start.first][start.second] == '#' || map[end.first][end.second] == '#')
		return false;
	int m = map.size(), n = map[0].size();
	vector<vector<bool>> visited(m, vector<bool>(n, false));
	queue<pair<int, int>> q;
	q.push(start);
	visited[start.first][start.second] = true;
	int dx[] = { 0, 1, -1, 0 };
	int dy[] = { 1, 0, 0, -1 };

	while (!q.empty())
	{
		pair<int, int> current = q.front();
		q.pop();
		int x = current.first;
		int y = current.second;
		if (x == end.first && y == end.second)
			return true;
		for (int i = 0; i < 4; ++i)
		{
			int nx = x + dx[i], ny = y + dy[i];
			if (nx >= 0 && nx < m && ny >= 0 && ny < n && !visited[nx][ny] && (map[nx][ny] == '.' || map[nx][ny] == 'B'))
			{
				visited[nx][ny] = true;
				q.push({ nx, ny });
			}
		}
	}
	return false;
}

bool dfs(int u, vector<vector<char>>& map, vector<pair<int, int>>& groupA, vector<pair<int, int>>& groupB, vector<bool>& visitedA, vector<int>& matchingB)
{
	visitedA[u] = true;
	for (int v = 0; v < groupB.size(); v++)
	{
		if (isReachable(map, groupA[u], groupB[v]))
		{
			if (matchingB[v] == -1 || (matchingB[v] != -1 && !visitedA[matchingB[v]] && dfs(matchingB[v], map, groupA, groupB, visitedA, matchingB)))
			{
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
	vector<int> matchingB(groupB.size(), -1);
	vector<pair<int, int>> mapping;

	for (int u = 0; u < n; u++) {
		fill(visitedA.begin(), visitedA.end(), false);
		dfs(u, map, groupA, groupB, visitedA, matchingB);
	}

	for (int v = 0; v < groupB.size(); v++) {
		if (matchingB[v] != -1) {
			// 保存A组和B组之间的匹配关系索引
			mapping.push_back({ matchingB[v], v });
		}
	}

	return mapping;
}
