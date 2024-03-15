#include "tools.h"

// 定义四个方向的移动
int directions[4][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };

// 检查当前位置是否有效
bool isValid(int x, int y, int m, int n, vector<vector<char>>& ground) {
    return x >= 0 && x < m && y >= 0 && y < n && (ground[x][y] == '.' || ground[x][y] == 'B');
}

void dfs_iterative(int x, int y, int m, int n, vector<vector<char>>& ground, vector<vector<bool>>& visited, vector<pair<int, int>>& block) {
    stack<pair<int, int>> stk;
    stk.push({ x, y });

    while (!stk.empty()) {
        auto [currX, currY] = stk.top();
        stk.pop();

        // 检查当前节点是否已经访问过，或者是否有效
        if (!isValid(currX, currY, m, n, ground) || visited[currX][currY]) continue;

        // 标记为已访问并加入当前块
        visited[currX][currY] = true;
        block.push_back({ currX, currY });

        // 遍历四个方向
        for (auto& dir : directions) {
            int newX = currX + dir[0];
            int newY = currY + dir[1];
            // 检查新的位置是否有效，如果有效则推入栈中
            if (isValid(newX, newY, m, n, ground) && !visited[newX][newY]) {
                stk.push({ newX, newY });
            }
        }
    }
}

// 主函数
vector<vector<pair<int, int>>> partitionGround(vector<vector<char>>& ground) {
    int m = ground.size(), n = ground[0].size();
    vector<vector<bool>> visited(m, vector<bool>(n, false));
    vector<vector<pair<int, int>>> result;

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if ((ground[i][j] == '.' || ground[i][j] == 'B') && !visited[i][j]) {
                vector<pair<int, int>> block;
                dfs_iterative(i, j, m, n, ground, visited, block);
                result.push_back(block);
            }
        }
    }

    return result;
}