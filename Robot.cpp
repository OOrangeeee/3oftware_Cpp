#include"classes.h"

Robot::Robot(int id, pair<int, int> pos, bool if_has, int status, pair<int, int> goal_pos, int berth_id, pair<int, int> berth_pos)
{
	this->ID = id;
	this->pos = pos;
	this->if_has = if_has;
	this->status = status;
	this->goal_pos = goal_pos;
	this->berth_id = berth_id;
	this->berth_pos = berth_pos;
	if_initPath = false;
	if_inBerth = false;
	path.clear();
	go_path.clear();
	back_path.clear();
	future_path.clear();
}

Robot::Robot()
{
	this->ID = -1;
	this->pos = make_pair(-1, -1);
	this->if_has = -1;
	this->status = -1;
	this->goal_pos = make_pair(-1, -1);
	this->berth_id = -1;
	this->berth_pos = make_pair(-1, -1);
	if_initPath = false;
	if_inBerth = false;
	path.clear();
	go_path.clear();
	back_path.clear();
	future_path.clear();
}

void Robot::resverPath()
{
	back_path = go_path;
	reverse(back_path.begin(), back_path.end());
	for (int i = 0; i < back_path.size(); i++)
	{
		if (back_path[i] == 0)
			back_path[i] = 1;
		else if (back_path[i] == 1)
			back_path[i] = 0;
		else if (back_path[i] == 2)
			back_path[i] = 3;
		else
			back_path[i] = 2;
	}
}

void Robot::initPath(vector<int> path)
{
	back_path.clear();
	go_path.clear();
	future_path.clear();
	this->path = path;
}

void Robot::update()
{
	if (pos == berth_pos)
	{
		if_inBerth = true;
	}

	if (path.empty() && !if_has && if_inBerth)
	{
		get_task();
	}

	if (!path.empty())
	{
		move();
	}
	else if (path.empty() && if_has && if_inBerth)
	{
		pull();
	}
	else if (path.empty() && !if_has && !if_inBerth)
	{
		get();
	}
}

void Robot::move()
{
	printf("move %d %d\n", ID, path[0]);
	fflush(stdout);
	path.erase(path.begin());
}

void Robot::get()
{
	printf("get %d", ID);
	fflush(stdout);
	if_has = true;
	path.clear();
	path = back_path;
	go_path.clear();
}

void Robot::pull()
{
	printf("pull %d", ID);
	fflush(stdout);
	if_has = false;
	path.clear();
	//放下后获取任务
	back_path.clear();

}

void Robot::get_task()
{
	if (!future_path.empty())
	{
		go_path = future_path;
		resverPath();
		path = go_path;
		future_path.clear();
	}
}