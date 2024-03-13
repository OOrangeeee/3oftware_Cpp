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

void Robot::initPath(const vector<int>& path)
{
	back_path.clear();
	go_path.clear();
	future_path.clear();
	this->path = path;
}

bool Robot::update()
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
	if (path.empty() && !if_has && if_inBerth && next_pos.first == -1 && next_pos.second == -1)
	{
		get_task();
	}

	if (!path.empty() && next_pos.first == -1 && next_pos.second == -1)
	{
		move();
	}
	if (path.empty() && !if_has && !if_inBerth && next_pos.first == -1 && next_pos.second == -1)
	{
		get();
	}
	if (path.empty() && if_has && if_inBerth && next_pos.first == -1 && next_pos.second == -1)
	{
		pull();
		return true;
	}
	return false;
}

void Robot::move()
{
	printf("move %d %d\n", ID, path[0]);
	fflush(stdout);
	get_next_pos();
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

void Robot::del_path()
{
	path.erase(path.begin());
}

void Robot::get_next_pos()
{
	if (path.empty())
	{
		next_pos = make_pair(-1, -1);
	}
	else if (path[0] == 0)
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

bool Robot::solve_error()
{
	//尝试改变路径解决冲突
	//先找到这个机器人现在要往哪个地方走，通过 path[0]即可
	//然后先考虑逆时针九十度的方向避让，然后考虑顺时针90°的方向避让，最后考虑往回走
	int now_dir = path[0];
	vector<pair<int, int>> plan_place;//计划可能去的地方
	if (now_dir == 0)//right
	{
		plan_place.push_back(make_pair(pos.first - 1, pos.second));
		plan_place.push_back(make_pair(pos.first + 1, pos.second));
		plan_place.push_back(make_pair(pos.first, pos.second - 1));
	}
	else if (now_dir == 1)//left
	{
		plan_place.push_back(make_pair(pos.first + 1, pos.second));
		plan_place.push_back(make_pair(pos.first - 1, pos.second));
		plan_place.push_back(make_pair(pos.first, pos.second + 1));
	}
	else if (now_dir == 2)//up
	{
		plan_place.push_back(make_pair(pos.first, pos.second - 1));
		plan_place.push_back(make_pair(pos.first, pos.second + 1));
		plan_place.push_back(make_pair(pos.first + 1, pos.second));
	}
	else//down
	{
		plan_place.push_back(make_pair(pos.first, pos.second + 1));
		plan_place.push_back(make_pair(pos.first, pos.second - 1));
		plan_place.push_back(make_pair(pos.first - 1, pos.second));
	}
	int plan_go = -1;
	int plan_back = -1;
	for (int i = 0; i < plan_place.size(); i++)
	{
		if (ground[plan_place[i].first][plan_place[i].second] == '.' || ground[plan_place[i].first][plan_place[i].second] == 'B')
		{
			if (plan_place[i].first != pos.first)
			{
				if (plan_place[i].first > pos.first)
				{
					plan_go = 3;
					plan_back = 2;
				}
				else
				{
					plan_go = 2;
					plan_back = 3;
				}
			}
			else
			{
				if (plan_place[i].second > pos.second)
				{
					plan_go = 0;
					plan_back = 1;
				}
				else
				{
					plan_go = 1;
					plan_back = 0;
				}
			}
		}
	}
	if (plan_go == -1)
	{
		return false;
	}
	path.insert(path.begin(), plan_back);
	path.insert(path.begin(), plan_go);
	return true;
}