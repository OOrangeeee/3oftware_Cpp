#include"classes.h"
#include"tools.h"
Solver::Solver()
{
	n = 200;
	robot_num = 10;
	berth_num = 10;
	boat_num = 5;
	N = 201;
	robots = vector<Robot>(robot_num);
	berths = vector<Berth>(berth_num);
	boats = vector<Boat>(boat_num);
	money = 0;
	boat_capacity = 0;
	id = 0;
	new_num = 0;
	if_getMatch = false;
}

//初始化程序
void Solver::init()
{

	//读取地图
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
		this->ground.push_back(tmp);
	}

	//读取港口
	for (int i = 1; i <= berth_num; i++)
	{
		int id = 0;
		scanf("%d", &id);
		int x, y;
		scanf("%d%d", &x, &y);
		pair<int, int> pos = make_pair(x, y);
		int time = 0;
		int speed = 0;
		scanf("%d%d", &time, &speed);
		berths.push_back(Berth(id, pos, time, speed));
	}

	//读取容量
	scanf("%d", &boat_capacity);

	char ok[5];
	scanf("%s", ok);

	//存下所有的A的位置
	findAndReplaceA(ground);
	getMatch();
	printf("OK\n");
	fflush(stdout);
}

//找到所有A的位置，并更新地图
void Solver::findAndReplaceA(vector<vector<char>>& ground)
{
	vector<pair<int, int>> A_positions_tmp;
	for (int i = 0; i < ground.size(); i++) {
		for (int j = 0; j < ground[i].size(); j++) {
			if (ground[i][j] == 'A') {
				A_positions_tmp.emplace_back(i, j);
				ground[i][j] = '.';
			}
		}
	}
	this->A_positions = A_positions_tmp;
}

//给每个港口尽可能匹配一个机器人,此时得到的是港口ID到现有机器人位置的索引的对应
void Solver::getMatch()
{
	vector<pair<int, int>> B;
	for (int i = 0; i < berth_num; i++)
	{
		B.push_back(berths[i].pos);
	}

	match_tmp = findBijectiveMapping(ground, B, A_positions);
}

//每一帧的输入
void Solver::everyInput()
{
	//读取帧数，钱数
	scanf("%d %d", &id, &money);

	//读取新货物数
	scanf("%d", &new_num);

	//读取新货物信息
	for (int i = 0; i < new_num; i++)
	{
		int x, y, val;
		scanf("%d %d", &x, &y, &val);
		int die_time = id + 1000;
		pair<int, int> pos = make_pair(x, y);
		getGood(pos, die_time, val);
	}
}

//读入新货物
void Solver::getGood(pair<int, int> pos, int die_time, int val)
{
	int min_dist = 1e7;
	vector<int> to_berth_path;
	int berthId = -1;
	for (int i = 0; i < berth_num; i++)
	{
		vector<int> tmp_path_g_to_b = findShortestPath(ground, pos, berths[i].pos);
		int tmp_dist = tmp_path_g_to_b.size();
		if (tmp_dist > 0)
		{
			if (tmp_dist < min_dist)
			{
				min_dist = tmp_dist;
				to_berth_path = tmp_path_g_to_b;
				berthId = i;
			}
		}
	}
	if (berthId == -1)
		return;
	berths[berthId].Good_future.insert(Good(pos, val, die_time, berthId, min_dist, val / min_dist));
}
