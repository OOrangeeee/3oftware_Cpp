#include "tools.h"

bool isReachable(const vector<vector<char>>& map, pair<int, int> start, pair<int, int> end) {
    if (map[start.first][start.second] == '*' || map[end.first][end.second] == '*' ||
        map[start.first][start.second] == '#' || map[end.first][end.second] == '#')
        return false;

    int m = map.size(), n = map[0].size();
    vector<vector<bool>> visited(m, vector<bool>(n, false));
    queue<pair<int, int>> q;
    q.push(start);
    visited[start.first][start.second] = true;
    int dx[] = { 0, 1, -1, 0 };
    int dy[] = { 1, 0, 0, -1 };

    while (!q.empty()) {
        pair<int, int> current = q.front();
        q.pop();
        int x = current.first;
        int y = current.second;
        if (x == end.first && y == end.second)
            return true;
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i], ny = y + dy[i];
            if (nx >= 0 && nx < m && ny >= 0 && ny < n && !visited[nx][ny] &&
                (map[nx][ny] == '.' || map[nx][ny] == 'B')) {
                visited[nx][ny] = true;
                q.push({ nx, ny });
            }
        }
    }
    return false;
}

bool dfs(int u, vector<vector<char>>& map, vector<pair<int, int>>& groupA, vector<pair<int, int>>& groupB,
    vector<bool>& visitedA, vector<vector<int>>& matchingB, vector<int>& matchCount) {
    visitedA[u] = true;
    for (int v = 0; v < groupB.size(); v++) {
        if (isReachable(map, groupA[u], groupB[v])) {
            if (matchingB[v].size() < matchCount[v]) {
                matchingB[v].push_back(u);
                return true;
            }
            else {
                for (int i = 0; i < matchingB[v].size(); i++) {
                    int iIndex = matchingB[v][i];
                    if (!visitedA[iIndex] && dfs(iIndex, map, groupA, groupB, visitedA, matchingB, matchCount)) {
                        matchingB[v][i] = u;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

vector<pair<int, int>> findBijectiveMapping(vector<vector<char>>& map, vector<pair<int, int>>& groupA, vector<pair<int, int>>& groupB) {
    int n = groupA.size();
    vector<bool> visitedA(n, false);
    vector<vector<int>> matchingB(groupB.size());
    vector<int> matchCount(groupB.size(), n / groupB.size());

    int extraMatches = n % groupB.size();
    for (int i = 0; i < extraMatches; ++i) {
        matchCount[i] += 1;
    }

    for (int u = 0; u < n; u++) {
        fill(visitedA.begin(), visitedA.end(), false);
        dfs(u, map, groupA, groupB, visitedA, matchingB, matchCount);
    }

    vector<pair<int, int>> mapping;
    for (int v = 0; v < groupB.size(); v++) {
        for (int u : matchingB[v]) {
            mapping.push_back({ u, v });
        }
    }
    return mapping;
}
