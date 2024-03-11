#include"tools.h"

struct Node 
{
	int x, y, cost, heuristic;
	bool operator>(const Node& other) const 
	{
		return cost + heuristic > other.cost + other.heuristic;
	}
};

int heuristic(pair<int, int> a, pair<int, int> b) 
{
	return abs(a.first - b.first) + abs(a.second - b.second);
}

vector<int> findShortestPath(vector<vector<char>>& map, pair<int, int> start, pair<int, int> end)
{
	int m = map.size(), n = map[0].size();
	vector<vector<int>> dist(m, vector<int>(n, -1));
	vector<vector<int>> prev(m, vector<int>(n, -1));
	priority_queue<Node, vector<Node>, greater<Node>> q;
	q.push({ start.first, start.second, 0, heuristic(start, end) });
	dist[start.first][start.second] = 0;
	int dx[] = { 0, 0, -1, 1 }; // ср,вС,ио,об
	int dy[] = { 1, -1, 0, 0 };
	while (!q.empty()) 
	{
		Node curr = q.top();
		q.pop();
		if (make_pair(curr.x, curr.y) == end) break;
		for (int i = 0; i < 4; ++i) 
		{
			int nx = curr.x + dx[i], ny = curr.y + dy[i];
			if (nx >= 0 && nx < m && ny >= 0 && ny < n && (map[nx][ny] != '#' && map[nx][ny] != '*') && dist[nx][ny] == -1) 
			{
				dist[nx][ny] = dist[curr.x][curr.y] + 1;
				prev[nx][ny] = i;
				q.push({ nx, ny, dist[nx][ny], heuristic({nx, ny}, end) });
			}
		}
	}

	if (dist[end.first][end.second] == -1) 
		return {};

	vector<int> path;
	pair<int, int> curr = end;
	while (curr != start) 
	{
		int direction = prev[curr.first][curr.second];
		path.push_back(direction);
		curr.first -= dx[direction];
		curr.second -= dy[direction];
	}
	reverse(path.begin(), path.end());
	return path;
}