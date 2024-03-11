#include"classes.h"

Berth::Berth(int id,pair<int,int> pos,int time,int speed)
{
	this->ID = id;
	this->pos = pos;
	this->time = time;
	this->speed = speed;
}

vector<int> Berth::give_task(int ID,vector<vector<char>> map)
{
	vector<int> task_path;
	task_path.clear();
	while (!Good_future.empty())
	{
		Good good = Good_future[0];
		Good_future.removeAt(0);
		task_path = findShortestPath(map, pos, good.pos);
		if (ID + task_path.size() + 2 >= good.dietime)
		{
			continue;
		}
		else
		{
			return task_path;
		}
	}
	return task_path;
}