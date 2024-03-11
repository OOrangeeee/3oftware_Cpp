#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <queue>
#include <utility>
#include <map>
#include <cstdlib> 
class PathFinder {
private:
    int m, n;
    const vector<vector<char>>& map;
    vector<vector<vector<int>>> dist;
    vector<vector<vector<pair<int, int>>>> prev;
    vector<int> dx = { 0, 0, -1, 1 }; // 右, 左, 上, 下
    vector<int> dy = { 1, -1, 0, 0 };

    struct Node {
        int x, y, cost, heuristic;
        bool operator>(const Node& other) const {
            return cost + heuristic > other.cost + other.heuristic;
        }
    };

    int heuristic(const pair<int, int>& a, const pair<int, int>& b) const {
        return abs(a.first - b.first) + abs(a.second - b.second);
    }

    void computeShortestPathFrom(int startX, int startY) {
        priority_queue<Node, vector<Node>, greater<Node>> pq;
        pq.push({ startX, startY, 0, 0 }); // 优化: 直接使用0替代heuristic计算
        dist[startX][startY][startX * n + startY] = 0;

        while (!pq.empty()) {
            Node cur = pq.top(); pq.pop();
            if (dist[cur.x][cur.y][startX * n + startY] < cur.cost) continue;

            for (int dir = 0; dir < 4; ++dir) {
                int nx = cur.x + dx[dir], ny = cur.y + dy[dir];
                if (nx >= 0 && nx < m && ny >= 0 && ny < n && (map[nx][ny] == '.' || map[nx][ny] == 'B') && dist[startX][startY][nx * n + ny] > cur.cost + 1) {
                    dist[startX][startY][nx * n + ny] = cur.cost + 1;
                    prev[nx][ny][startX * n + startY] = { cur.x, cur.y };
                    pq.push({ nx, ny, dist[startX][startY][nx * n + ny], heuristic({nx, ny}, {startX, startY}) });
                }
            }
        }
    }

public:
    PathFinder(const vector<vector<char>>& map) : map(map) {
        m = map.size();
        n = map[0].size();
        dist.resize(m, vector<vector<int>>(n, vector<int>(m * n, numeric_limits<int>::max())));
        prev.resize(m, vector<vector<pair<int, int>>>(n, vector<pair<int, int>>(m * n, { -1, -1 })));

        for (int x = 0; x < m; ++x) {
            for (int y = 0; y < n; ++y) {
                if (map[x][y] == '.' || map[x][y] == 'B') {
                    computeShortestPathFrom(x, y);
                }
            }
        }
    }

    vector<int> findShortestPath(const pair<int, int>& start, const pair<int, int>& end) {
        vector<int> path;
        if (dist[start.first][start.second][end.first * n + end.second] == numeric_limits<int>::max()) {
            return path; // No path found, return an empty vector
        }

        int curX = end.first, curY = end.second;
        while (!(curX == start.first && curY == start.second)) {
            pair<int, int> prevPoint = prev[curX][curY][start.first * n + start.second];
            int dir = findDirection(prevPoint, { curX, curY });
            path.push_back(dir);
            curX = prevPoint.first;
            curY = prevPoint.second;
        }

        reverse(path.begin(), path.end());
        return path;
    }

    int findDirection(const pair<int, int>& from, const pair<int, int>& to) const {
        for (int dir = 0; dir < 4; ++dir) {
            if (from.first + dx[dir] == to.first && from.second + dy[dir] == to.second) {
                return dir;
            }
        }
        return -1; // Should not happen, indicates an error
    }
};