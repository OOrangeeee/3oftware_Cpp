#include"classes.h"

Berth::Berth(int id, pair<int, int> pos, int time, int speed)
{
	this->ID = id;
	this->pos = pos;
	this->time = time;
	this->speed = speed;
	this->BoatId = -1;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			poses.push_back(make_pair(pos.first + i, pos.second + j));
		}
	}
}

Berth::Berth()
{
	this->ID = -1;
	this->pos = make_pair(-1, -1);
	this->time = -1;
	this->speed = -1;
	this->BoatId = -1;
}

vector<int> Berth::give_task(int ID, vector<vector<char>> map, pair<int, int> now_pos_berth)
{
	vector<int> task_path;
	task_path.clear();
	while (!Good_future.empty())
	{
		Good good = Good_future[0];
		Good_future.removeAt(0);
		task_path = findShortestPath(map, now_pos_berth, good.pos);
		if (ID + task_path.size() + 2 >= good.dietime)
		{
			task_path.clear();
			continue;
		}
		else
		{
			break;
		}
	}
	return task_path;
}