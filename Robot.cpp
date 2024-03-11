#include"classes.h"

Robot::Robot(int id, pair<int, int> pos, bool if_has, int status, pair<int, int> goal_pos, int berth_id, pair<int, int> berth_pos, vector<vector<char>> ground)
{
	this->ID = id;
	this->pos = pos;
	this->if_has = if_has;
	this->status = status;
	this->goal_pos = goal_pos;
	this->berth_id = berth_id;
	this->berth_pos = berth_pos;
	this->ground = ground;
	if_initPath = false;
	if_inBerth = false;
	path.clear();
	go_path.clear();
	back_path.clear();
	future_path.clear();
	next_pos = make_pair(-1, -1);
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
	next_pos = make_pair(-1, -1);
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
	if (pos == next_pos)
	{
		next_pos = make_pair(-1, -1);
		del_path();
	}
	if (pos == berth_pos)
	{
		if_inBerth = true;
	}
	else
	{
		if_inBerth = false;
	}
	/*if (path.empty())
		printf("++++++++++++++++++++++++++++++++++++++++++++\n");*/
	if (path.empty() && !if_has && if_inBerth && next_pos.first == -1 && next_pos.second == -1)
	{
		get_task();
	}

	if (!path.empty() && next_pos.first == -1 && next_pos.second == -1)
	{
		move();
	}
	if (path.empty() && if_has && if_inBerth && next_pos.first == -1 && next_pos.second == -1)
	{
		pull();
	}
	if (path.empty() && !if_has && !if_inBerth && next_pos.first == -1 && next_pos.second == -1)
	{
		get();
	}
}

void Robot::move()
{
	printf("move %d %d\n", ID, path[0]);
	fflush(stdout);
	if (path[0] == 0)
	{
		next_pos = make_pair(pos.first, pos.second + 1);
	}
	else if (path[0] == 1)
	{
		next_pos = make_pair(pos.first, pos.second - 1);
	}
	else if (path[0] == 2)
	{
		next_pos = make_pair(pos.first - 1, pos.second);
	}
	else if (path[0] == 3)
	{
		next_pos = make_pair(pos.first + 1, pos.second);
	}
}

void Robot::get()
{
	printf("get %d\n", ID);
	fflush(stdout);
	if_has = true;
	path.clear();
	path = back_path;
	go_path.clear();
	//next_pos = make_pair(-500, -500);
}

void Robot::pull()
{
	printf("pull %d\n", ID);
	fflush(stdout);
	if_has = false;
	path.clear();
	//���º��ȡ����
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

void Robot::del_path()
{
	path.erase(path.begin());
}