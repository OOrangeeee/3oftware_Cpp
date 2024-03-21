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
	if_chuchai = false;
	path.clear();
	go_path.clear();
	back_path.clear();
	//future_path.clear();
	next_pos = make_pair(-1, -1);
	pre_error = 0;
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
	if_chuchai = false;
	path.clear();
	go_path.clear();
	back_path.clear();
	//future_path.clear();
	next_pos = make_pair(-1, -1);
	pre_error = 0;
}

void Robot::reversePath()
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
	//future_path.clear();
	this->path = path;
}

bool Robot::update()
{
	if (pre_error >= 400)
	{
		vector<int> tmp_path = findShortestPath(ground, pos, berth_pos);
		vector<int> tmp_path_back = tmp_path;
		reverse(tmp_path_back.begin(), tmp_path_back.end());
		for (int i = 0; i < tmp_path_back.size(); i++)
		{
			if (tmp_path_back[i] == 0)
				tmp_path_back[i] = 1;
			else if (tmp_path_back[i] == 1)
				tmp_path_back[i] = 0;
			else if (tmp_path_back[i] == 2)
				tmp_path_back[i] = 3;
			else
				tmp_path_back[i] = 2;
		}
		for (int j = tmp_path_back.size() - 1; j >= 0; j--)
		{
			path.insert(path.begin(), tmp_path_back[j]);
		}
		for (int j = tmp_path.size() - 1; j >= 0; j--)
		{
			path.insert(path.begin(), tmp_path[j]);
		}
		pre_error = 0;
	}
	if (pos == next_pos)
	{
		next_pos = make_pair(-1, -1);
		now_dir = -1;
	}
	else if (next_pos.first != -1 && now_dir != -1)
	{
		printf("move %d %d\n", ID, now_dir);
		return false;
	}
	if (pos == berth_pos)
	{
		if_inBerth = true;
	}
	else
	{
		if_inBerth = false;
	}
	//if (path.empty() && !if_has && if_inBerth && next_pos.first == -1 && next_pos.second == -1)
	//{
	//	get_task();
	//}

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
	now_dir = del_path();
}

void Robot::get()
{
	printf("get %d\n", ID);
	fflush(stdout);
	path.clear();
	path = back_path;
	go_path.clear();
	pre_error = 0;
	//next_pos = make_pair(-500, -500);
}

void Robot::pull()
{
	printf("pull %d\n", ID);
	fflush(stdout);
	path.clear();
	//放下后获取任务
	back_path.clear();
	pre_error = 0;

}

//void Robot::get_task()
//{
//	if (!future_path.empty())
//	{
//		go_path = future_path;
//		reversePath();
//		path = go_path;
//		future_path.clear();
//	}
//}

int Robot::del_path()
{
	int dir = path[0];
	path.erase(path.begin());
	return dir;
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

bool Robot::solve_error(pair<int, int> other_pos)
{
	//尝试改变路径解决冲突
	//先找到这个机器人现在要往哪个地方走，通过 path[0]即可
	//然后先考虑逆时针九十度的方向避让，然后考虑顺时针90°的方向避让，最后考虑往回走
	vector<pair<int, int>> plan_place;//计划可能去的地方
	int plan_go = -1;
	int plan_back = -1;
	if (path.empty())
	{
		if (pos.first - 1 >= 0)
		{
			plan_place.push_back(make_pair(pos.first - 1, pos.second));
		}
		if (pos.first <= 199)
		{
			plan_place.push_back(make_pair(pos.first + 1, pos.second));
		}
		if (pos.second - 1 >= 0)
		{
			plan_place.push_back(make_pair(pos.first, pos.second - 1));
		}
		if (pos.second + 1 >= 0)
		{
			plan_place.push_back(make_pair(pos.first, pos.second + 1));
		}
		for (int i = 0; i < plan_place.size(); i++)
		{
			if ((ground[plan_place[i].first][plan_place[i].second] == '.' || ground[plan_place[i].first][plan_place[i].second] == 'B') && plan_place[i].first != other_pos.first && plan_place[i].second != other_pos.second)
			{
				if (plan_place[i].first != pos.first)
				{
					if (plan_place[i].first > pos.first)
					{
						plan_go = 3;
						plan_back = 2;
						break;
					}
					else
					{
						plan_go = 2;
						plan_back = 3;
						break;
					}
				}
				else
				{
					if (plan_place[i].second > pos.second)
					{
						plan_go = 0;
						plan_back = 1;
						break;
					}
					else
					{
						plan_go = 1;
						plan_back = 0;
						break;
					}
				}
			}
		}
		if (plan_go == -1)
		{
			pre_error++;
			return false;
		}
		path.insert(path.begin(), plan_back);
		path.insert(path.begin(), plan_go);
		pre_error++;
		return true;
	}
	else
	{
		int now_dir_error = path[0];
		if (now_dir_error == 0)//right
		{
			if (pos.first - 1 >= 0)
			{
				plan_place.push_back(make_pair(pos.first - 1, pos.second));
			}
			if (pos.first + 1 <= 199)
			{
				plan_place.push_back(make_pair(pos.first + 1, pos.second));
			}
			if (pos.second - 1 >= 0)
			{
				plan_place.push_back(make_pair(pos.first, pos.second - 1));
			}

		}
		else if (now_dir_error == 1)//left
		{
			if (pos.first + 1 <= 199)
			{
				plan_place.push_back(make_pair(pos.first + 1, pos.second));
			}
			if (pos.first - 1 >= 0)
			{
				plan_place.push_back(make_pair(pos.first - 1, pos.second));
			}
			if (pos.second + 1 <= 199)
			{
				plan_place.push_back(make_pair(pos.first, pos.second + 1));
			}
		}
		else if (now_dir_error == 2)//up
		{
			if (pos.second - 1 >= 0)
			{
				plan_place.push_back(make_pair(pos.first, pos.second - 1));
			}
			if (pos.second + 1 <= 199)
			{
				plan_place.push_back(make_pair(pos.first, pos.second + 1));
			}
			if (pos.first + 1 <= 199)
			{
				plan_place.push_back(make_pair(pos.first + 1, pos.second));
			}
		}
		else//down
		{
			if (pos.second + 1 <= 199)
			{
				plan_place.push_back(make_pair(pos.first, pos.second + 1));
			}
			if (pos.second - 1 >= 0)
			{
				plan_place.push_back(make_pair(pos.first, pos.second - 1));
			}
			if (pos.first - 1 >= 0)
			{
				plan_place.push_back(make_pair(pos.first - 1, pos.second));
			}
		}
		int now_choice = pre_error % 4;
		if (now_choice == 0)
		{

		}
		else if (now_choice == 1 && plan_place.size() >= 1)
		{
			pair<int, int> one = plan_place[0];
			plan_place.erase(plan_place.begin());
			plan_place.push_back(one);
		}
		else if (now_choice == 2 && plan_place.size() >= 2)
		{
			pair<int, int> one = plan_place[0];
			pair<int, int> two = plan_place[1];
			plan_place.erase(plan_place.begin());
			plan_place.erase(plan_place.begin());
			plan_place.push_back(one);
			plan_place.push_back(two);
		}
		else if (now_choice == 3 && plan_place.size() >= 2)
		{
			pair<int, int> one = plan_place[0];
			pair<int, int> two = plan_place[1];
			plan_place.erase(plan_place.begin());
			plan_place.erase(plan_place.begin());
			plan_place.push_back(two);
			plan_place.push_back(one);
		}

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
						break;
					}
					else
					{
						plan_go = 2;
						plan_back = 3;
						break;
					}
				}
				else
				{
					if (plan_place[i].second > pos.second)
					{
						plan_go = 0;
						plan_back = 1;
						break;
					}
					else
					{
						plan_go = 1;
						plan_back = 0;
						break;
					}
				}
			}
		}
		if (plan_go == -1)
		{
			pre_error++;
			return false;
		}
		path.insert(path.begin(), plan_back);
		path.insert(path.begin(), plan_go);
		pre_error++;
		return true;
	}
}