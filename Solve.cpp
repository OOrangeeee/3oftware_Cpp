#include"classes.h"
#include"tools.h"

Solver::Solver()
{
	n = 200;
	robot_num = 10;
	berth_num = 10;
	boat_num = 5;
	N = 201;
	robots.reserve(robot_num);
	berths.reserve(berth_num);
	boats.reserve(boat_num);
	money = 0;
	boat_capacity = 0;
	id = 0;
	new_num = 0;
	if_getMatch = false;
}

void Solver::success()
{

}

//初始化程序
void Solver::init()
{
	//初始化机器人
	for (int i = 0; i < robot_num; i++)
	{
		robots[i] = Robot(i, make_pair(-1, -1), false, 1, make_pair(-1, -1), -1, make_pair(-1, -1));
	}

	//初始化船
	for (int i = 0; i < boat_num; i++)
	{
		boats[i] = Boat(-1, -1);
	}

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
		berths[id] = Berth(id, pos, time, speed);
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

	//读取机器人信息
	for (int i = 0; i < robot_num; i++)
	{
		int if_has_int;
		int x;
		int y;
		int status;
		scanf("%d %d %d %d", &if_has_int, &x, &y, &status);
		pair<int, int> now_pos = make_pair(x, y);
		robots[i].if_has = if_has_int == 1 ? true : false;
		robots[i].pos = now_pos;
		robots[i].status = status;
		if (!if_getMatch)
		{
			A_roubt.push_back(make_pair(now_pos, i));
		}
	}

	//把能对应的港口和机器人对应起来
	if (!if_getMatch)
	{
		get_match();
		//把能对应的机器人和港口保存起来
		for (int i = 0; i < robot_num; i++)
		{
			for (int j = 0; j < match_rb.size(); j++)
			{
				if (i == match_rb[j].first)
				{
					robots[i].berth_id = match_rb[j].second;
					robots[i].berth_pos = berths[match_rb[j].second].pos;
				}
			}
			for (int j = 0; j < match_br.size(); j++)
			{
				if (i == match_br[j].first)
				{
					berths[i].RobotId = match_br[j].second;
				}
			}
		}
	}

	//读取船信息
	for (int i = 0; i < boat_num; i++)
	{
		int status, goal;
		scanf("%d %d", &status, &goal);
		boats[i].goal = goal;
		boats[i].status = status;
	}

	char ok[5];
	scanf("%s", ok);
}

void Solver::action()
{
	//布置任务
	for (int i = 0; i < berth_num; i++)
	{
		if (berths[i].RobotId != -1)
		{
			if (!berths[i].Good_future.empty() && robots[berths[i].RobotId].future_path.empty())
			{
				vector<int> tmp_path = berths[i].give_task(id, ground);
				if (!tmp_path.empty())
				{
					robots[berths[i].RobotId].future_path = tmp_path;
				}
			}
		}
	}

	//指示机器人
	for (int i = 0; i < robot_num; i++)
	{
		if (robots[i].berth_id != -1)//有用的机器人
		{
			if (robots[i].status == 1)
			{
				if (robots[i].if_initPath == false)
				{
					robots[i].initPath(findShortestPath(ground, robots[i].pos, robots[i].berth_pos));
					robots[i].if_initPath = true;
				}
				robots[i].update();
			}
		}
	}


}

void Solver::get_match()
{
	for (int i = 0; i < match_tmp.size(); i++)
	{
		int index = match_tmp[i].second;
		pair<int, int> tmp_pos = A_positions[index];
		for (int j = 0; j < A_roubt.size(); i++)
		{
			if (A_roubt[j].first.first == tmp_pos.first && A_roubt[j].first.second == tmp_pos.second)
			{
				int roubtId = A_roubt[j].second;
				match_br.push_back(make_pair(match_tmp[i].first, roubtId));
				match_rb.push_back(make_pair(roubtId, match_tmp[i].first));
				break;
			}
		}
	}
	if_getMatch = true;
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

	match_tmp = findBijectiveMapping(ground, B, A_positions);//港口id 机器人位置索引
}



//读入新货物
void Solver::getGood(pair<int, int> pos, int die_time, int val)
{
	int min_dist = 1e7;
	int berthId = -1;
	for (int i = 0; i < berth_num; i++)
	{
		int dist = manhattanDistance(pos, berths[i].pos);
		if (dist < min_dist)
		{
			min_dist = dist;
			berthId = i;
		}

	}
	if (berthId == -1)
		return;
	berths[berthId].Good_future.insert(Good(pos, val, die_time, berthId, min_dist, val / min_dist));
}
