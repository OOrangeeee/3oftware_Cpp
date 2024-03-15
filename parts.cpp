#include "tools.h"

// �����ĸ�������ƶ�
int directions[4][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };

// ��鵱ǰλ���Ƿ���Ч
bool isValid(int x, int y, int m, int n, vector<vector<char>>& ground) {
    return x >= 0 && x < m && y >= 0 && y < n && (ground[x][y] == '.' || ground[x][y] == 'B');
}

void dfs_iterative(int x, int y, int m, int n, vector<vector<char>>& ground, vector<vector<bool>>& visited, vector<pair<int, int>>& block) {
    stack<pair<int, int>> stk;
    stk.push({ x, y });

    while (!stk.empty()) {
        auto [currX, currY] = stk.top();
        stk.pop();

        // ��鵱ǰ�ڵ��Ƿ��Ѿ����ʹ��������Ƿ���Ч
        if (!isValid(currX, currY, m, n, ground) || visited[currX][currY]) continue;

        // ���Ϊ�ѷ��ʲ����뵱ǰ��
        visited[currX][currY] = true;
        block.push_back({ currX, currY });

        // �����ĸ�����
        for (auto& dir : directions) {
            int newX = currX + dir[0];
            int newY = currY + dir[1];
            // ����µ�λ���Ƿ���Ч�������Ч������ջ��
            if (isValid(newX, newY, m, n, ground) && !visited[newX][newY]) {
                stk.push({ newX, newY });
            }
        }
    }
}

// ������
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