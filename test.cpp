#include"tools.h"

int main() {

	vector<pair<int, int>> A;
	vector<pair<int, int>> B;

	A.push_back(make_pair(16, 58));
	A.push_back(make_pair(56, 10));
	A.push_back(make_pair(54, 99));
	A.push_back(make_pair(36, 44));
	A.push_back(make_pair(5, 20));
	A.push_back(make_pair(62, 85));
	A.push_back(make_pair(23, 88));
	A.push_back(make_pair(87, 2));
	A.push_back(make_pair(69, 88));
	A.push_back(make_pair(16, 17));

	B.push_back(make_pair(36, 173));
	B.push_back(make_pair(55, 89));
	B.push_back(make_pair(64, 75));
	B.push_back(make_pair(108, 74));
	B.push_back(make_pair(108, 127));
	B.push_back(make_pair(130, 141));
	B.push_back(make_pair(163, 62));
	B.push_back(make_pair(165, 128));
	B.push_back(make_pair(181, 28));
	B.push_back(make_pair(193, 124));

	vector<vector<char>> map;

	char ch;
	for (int i = 1; i <= 200; i++)
	{
		vector<char> tmp;
		while (scanf("%c", &ch) == 1) {
			if (ch == '\n') {
				break;
			}
			tmp.push_back(ch);
		}
		map.push_back(tmp);
	}


	for (vector<char> v : map)
	{
		for (int i = 0; i < v.size(); i++)
			printf("%c", v[i]);
		printf("\n");
	}

	return 0;
}