#include"tools.h"
vector<int> findShortestPath(vector<vector<char>>& map, pair<int, int> start, pair<int, int> end)
{
	int m = map.size();
	int n = map[0].size();
	vector<vector<int>> dist(m, vector<int>(n, -1));
	vector<vector<int>> prev(m, vector<int>(n, -1));
	queue<pair<int, int>> q;
	q.push(start);
	dist[start.first][start.second] = 0;
	int dx[] = { 0, 0, -1, 1 };  // ср,вС,ио,об
	int dy[] = { 1, -1, 0, 0 };
	while (!q.empty()) {
		pair<int, int> curr = q.front();
		q.pop();
		if (curr == end) {
			break;
		}
		for (int i = 0; i < 4; i++) {
			int nx = curr.first + dx[i];
			int ny = curr.second + dy[i];
			if (nx >= 0 && nx < m && ny >= 0 && ny < n && (map[nx][ny] == '.' || map[nx][ny] == 'B') && dist[nx][ny] == -1) {
				dist[nx][ny] = dist[curr.first][curr.second] + 1;
				prev[nx][ny] = i;
				q.push({ nx, ny });
			}
		}
	}
	if (dist[end.first][end.second] == -1) {
		return {};
	}
	vector<int> path;
	pair<int, int> curr = end;
	while (curr != start) {
		int direction = prev[curr.first][curr.second];
		path.push_back(direction);
		curr.first -= dx[direction];
		curr.second -= dy[direction];
	}
	reverse(path.begin(), path.end());
	return path;
}