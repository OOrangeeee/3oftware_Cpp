#include"classes.h"
#include"tools.h"

Solver::Solver()
{
	n = 200;
	robot_num = 10;
	berth_num = 10;
	boat_num = 5;
	N = 201;
	robots.resize(robot_num);
	berths.resize(berth_num);
	boats.resize(boat_num);
	money = 0;
	boat_capacity = 0;
	id = 0;
	new_num = 0;
	berths_dist = 0;
	if_getMatch = false;
	error_no = 0;
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

	//存下所有的A的位置
	findAndReplaceA(ground);

	//初始化机器人
	for (int i = 0; i < robot_num; i++)
	{
		robots[i] = Robot(i, make_pair(-1, -1), false, 1, make_pair(-1, -1), -1, make_pair(-1, -1), ground);
		next_point_for_Robots.push_back(make_pair(-1, -1));
	}

	//初始化船
	for (int i = 0; i < boat_num; i++)
	{
		boats[i] = Boat(-1, -1);
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

	for (int i = 0; i < boat_num; i++)
	{
		boats[i].can_have = boat_capacity;
		boats[i].id = i;
	}

	for (int i = 0; i < berth_num; i++)
	{
		berths[i].price = 2 * berths[i].time + (boat_capacity / berths[i].speed);
	}
	char ok[5];
	scanf("%s", ok);
	printf("OK\n");
	fflush(stdout);
}

//每一帧的输入
void Solver::everyInput()
{
	if (cin.eof())
	{
		exit(0);
	}

	//读取帧数，钱数
	scanf("%d %d", &id, &money);

	//读取新货物数
	scanf("%d", &new_num);

	//读取新货物信息
	for (int i = 0; i < new_num; i++)
	{
		int x, y, val;
		scanf("%d %d %d", &x, &y, &val);
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
	}

	//把能对应的港口和机器人对应起来
	if (!if_getMatch)
	{
		get_berths();
		get_Boat_Berth_match();
		if_getMatch = true;
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
		for (int j = 0; j < berths[i].RobotIdS.size(); j++)
		{
			int RobotId = berths[i].RobotIdS[j];
			if (!berths[i].Good_future.empty() && robots[RobotId].path.empty() && robots[RobotId].if_inBerth && !robots[RobotId].if_has && robots[RobotId].next_pos.first == -1 && robots[RobotId].now_dir == -1)
			{
				pair<int, int> now_pos_berth = robots[RobotId].berth_pos;
				vector<int> tmp_path = berths[i].give_task(id, ground, now_pos_berth);
				if (!tmp_path.empty())
				{
					robots[RobotId].path = tmp_path;
					robots[RobotId].go_path = tmp_path;
					robots[RobotId].reversePath();
					robots[RobotId].pre_error = 0;
				}
			}
		}
	}

	//指示机器人
	check_error();
	for (int i = 0; i < robot_num; i++)
	{
		if (robots[i].berth_id == -1)
		{
			if (!robots[i].path.empty())
			{
				robots[i].update();
			}
			else
			{
				robots[i].next_pos = make_pair(-1, -1);
				robots[i].now_dir = -1;
			}
		}
		else//有用的机器人
		{
			if (robots[i].status == 1)
			{
				if (!robots[i].if_initPath)
				{
					robots[i].initPath(findShortestPath(ground, robots[i].pos, robots[i].berth_pos));
					robots[i].if_initPath = true;
				}
				if (robots[i].update())
				{
					int b_id = robots[i].berth_id;
					if (boats[berths[b_id].BoatId].berthId_1 == b_id)
					{
						boats[berths[b_id].BoatId].berthId_1_num++;
					}
					else if (boats[berths[b_id].BoatId].berthId_2 != -1)
					{
						boats[berths[b_id].BoatId].berthId_2_num++;
					}
				}
			}
		}
	}

	//指示船
	for (int i = 0; i < boat_num; i++)
	{
		boats[i].zhen = id;
		if (boats[i].goal < 0 && boats[i].status == 1)
		{
			if (boats[i].berthId_2_num > 0 && boats[i].berthId_1_num == 0)
			{
				boats[i].ship(boats[i].berthId_2);
			}
			else
			{
				boats[i].ship(boats[i].berthId_1);
			}
		}
		else if (boats[i].status == 1 && boats[i].berthId_1 != -1)
		{
			boats[i].update();
		}
	}

}

//void Solver::get_match()
//{
//	for (int i = 0; i < match_tmp.size(); i++)
//	{
//		int index1 = match_tmp[i].first;
//		int index2 = match_tmp[i].second;
//		pair<int, int> tmp_pos = A_positions[index1];
//		for (int j = 0; j < A_roubt.size(); j++)
//		{
//			if (A_roubt[j].first.first == tmp_pos.first && A_roubt[j].first.second == tmp_pos.second)
//			{
//				int roubtId = A_roubt[j].second;
//				match_br.push_back(make_pair(index2, roubtId));
//				match_rb.push_back(make_pair(roubtId, index2));
//				break;
//			}
//		}
//	}
//	if_getMatch = true;
//}

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

////给每个港口尽可能匹配一个机器人,此时得到的是港口ID到现有机器人位置的索引的对应
//void Solver::getMatchTmp()
//{
//	vector<pair<int, int>> B;
//	vector<pair<int, int>> tmp;
//	for (int i = 0; i < boats.size(); i++)
//	{
//		B.push_back(berths[boats[i].berthId_1].pos);
//		tmp.push_back(make_pair(i, boats[i].berthId_1));
//	}
//	vector<pair<int, int>> match_tmp_f = findBijectiveMapping(ground, A_positions, B);//机器人位置 港口id 索引
//	for (int i = 0; i < match_tmp_f.size(); i++)
//	{
//		for (int j = 0; j < tmp.size(); j++)
//		{
//			if (match_tmp_f[i].second == tmp[j].first)
//			{
//				match_tmp.push_back(make_pair(match_tmp_f[i].first, tmp[j].second));
//				break;
//			}
//		}
//	}
//}



//读入新货物
void Solver::getGood(pair<int, int> pos, int die_time, int val)
{
	//berths_busy.clear();
	//for (int i = 0; i < using_berth.size(); i++)
	//{
	//	berths_busy.push_back(berths[using_berth[i]].Good_future.size());
	//}
	//sort(berths_busy.begin(), berths_busy.end());
	//int tmp_busy = berths_busy[using_berth.size() / 2];
	int min_dist = 1e7;
	int berthId = -1;
	for (int i = 0; i < using_berth.size(); i++)
	{
		int dist = manhattanDistance(pos, berths[using_berth[i]].pos);
		//if (dist <= berths_dist && berths[using_berth[i]].Good_future.size() < tmp_busy)
		//{
		//	min_dist = dist;
		//	berthId = using_berth[i];
		//	ans_path = tmp_path;
		//	break;
		//}
		if (dist < min_dist)
		{
			min_dist = dist;
			berthId = using_berth[i];
		}

	}
	if (berthId == -1)
		return;
	berths[berthId].Good_future.insert(Good(pos, val, die_time, berthId, min_dist, val / min_dist));
}

void Solver::get_Boat_Berth_match()
{
	int berths_using_num = using_berth.size();
	int length = min(boat_num, berths_using_num);
	for (int i = 0; i < length; i++)
	{
		boats[i].berthId_1 = berths[using_berth[i]].ID;
		boats[i].berthId_1_speed = berths[using_berth[i]].speed;
		boats[i].berthId_1_time = berths[using_berth[i]].time;
		berths[using_berth[i]].BoatId = i;
	}
	if (boat_num > length)
	{
		for (int i = length, j = 0; i < boat_num; i++, j++)
		{
			j = j % berths_using_num;
			boats[i].berthId_1 = berths[using_berth[j]].ID;
			boats[i].berthId_1_speed = berths[using_berth[j]].speed;
			boats[i].berthId_1_time = berths[using_berth[j]].time;
			berths[using_berth[j]].BoatId = i;
		}
	}
	if (berths_using_num > length)
	{
		for (int i = berths_using_num - 1, j = 0; i >= length; i--, j++)
		{
			j = j % boat_num;
			boats[j].berthId_2 = berths[using_berth[i]].ID;
			boats[j].berthId_2_time = berths[using_berth[i]].time;
			boats[j].berthId_2_speed = berths[using_berth[i]].speed;
			berths[using_berth[i]].BoatId = j;
		}
	}
}

void Solver::check_error()
{
	vector<pair<int, int>> now_pos;
	get_next_pos();
	for (int i = 0; i < robot_num; i++)
	{
		now_pos.push_back(robots[i].pos);
	}
	vector<pair<int, int>> error_1 = findAllPairDuplicates(next_point_for_Robots);
	vector<pair<int, int>> error_2 = find_equal_pairs(now_pos, next_point_for_Robots);
	vector<pair<int, int>> error_3 = check_error_for_berth(now_pos, next_point_for_Robots);
	vector<pair<int, int>> error = merge_vectors(error_1, error_2);
	vector<int> del;
	for (int i = 0; i < error.size(); i++)
	{
		int error_robot_id_1 = min(error[i].first, error[i].second);
		int error_robot_id_2 = max(error[i].first, error[i].second);
		if (error_robot_id_1 == error_robot_id_2 || next_point_for_Robots[error_robot_id_2].first == -1 || next_point_for_Robots[error_robot_id_2].second == -1 || next_point_for_Robots[error_robot_id_1].first == -1 || next_point_for_Robots[error_robot_id_1].second == -1)
		{
			del.push_back(i);
		}
	}
	removeIndices_for_4(error, del);
	del.clear();
	//error = merge_vectors(error_3, error);
	int count = 0;
	while (!error.empty() || !error_3.empty())
	{
		if (count > 100)
		{
			for (int i = 0; i < using_berth.size(); i++)
			{
				pair<int, int> to_pos;
				for (int j = 0; j < parts.size(); j++)
				{
					if (find(parts[j].begin(), parts[j].end(), robots[using_berth[i]].pos) != parts[j].end())
					{
						int num = getRandomNumber(0, parts[j].size());
						to_pos = parts[j][num];
					}
				}
				vector<int> tmp_path = findShortestPath(ground, robots[using_berth[i]].pos, to_pos);
				vector<int> tmp_path_back = tmp_path;
				reverse(tmp_path_back.begin(), tmp_path_back.end());
				for (int j = 0; j < tmp_path_back.size(); j++)
				{
					if (tmp_path_back[j] == 0)
						tmp_path_back[j] = 1;
					else if (tmp_path_back[j] == 1)
						tmp_path_back[j] = 0;
					else if (tmp_path_back[j] == 2)
						tmp_path_back[j] = 3;
					else
						tmp_path_back[j] = 2;
				}
				for (int j = tmp_path_back.size() - 1; j >= 0; j--)
				{
					robots[using_berth[i]].path.insert(robots[using_berth[i]].path.begin(), tmp_path_back[j]);
				}
				for (int j = tmp_path.size() - 1; j >= 0; j--)
				{
					robots[using_berth[i]].path.insert(robots[using_berth[i]].path.begin(), tmp_path[j]);
				}
				robots[using_berth[i]].pre_error = 0;
			}
			get_next_pos();
			error_1 = findAllPairDuplicates(next_point_for_Robots);
			error_2 = find_equal_pairs(now_pos, next_point_for_Robots);
			error_3 = check_error_for_berth(now_pos, next_point_for_Robots);
			error = merge_vectors(error_1, error_2);
			for (int i = 0; i < error.size(); i++)
			{
				int error_robot_id_1 = min(error[0].first, error[0].second);
				int error_robot_id_2 = max(error[0].first, error[0].second);
				if (error_robot_id_1 == error_robot_id_2 || next_point_for_Robots[error_robot_id_2].first == -1 || next_point_for_Robots[error_robot_id_2].second == -1 || next_point_for_Robots[error_robot_id_1].first == -1 || next_point_for_Robots[error_robot_id_1].second == -1)
				{
					del.push_back(i);
				}
			}
			removeIndices_for_4(error, del);
			del.clear();
		}
		while (!error.empty())
		{
			int error_robot_id_1 = min(error[0].first, error[0].second);
			int error_robot_id_2 = max(error[0].first, error[0].second);
			error.erase(error.begin());
			if (error_robot_id_1 != error_robot_id_2 && next_point_for_Robots[error_robot_id_2].first != -1 && next_point_for_Robots[error_robot_id_2].second != -1 && next_point_for_Robots[error_robot_id_1].first != -1 && next_point_for_Robots[error_robot_id_1].second != -1)
			{
				//真冲突了
				//尝试用第一个机器人解决冲突
				bool num1_success = robots[error_robot_id_1].solve_error(robots[error_robot_id_2].pos);
				if (num1_success)
				{
					count++;
					continue;
				}
				else
				{
					count++;
					bool num2_success = robots[error_robot_id_2].solve_error(robots[error_robot_id_1].pos);
				}
			}
		}
		while (!error_3.empty())
		{
			int error_robot_id_1 = error_3[0].first;
			int error_robot_id_2 = error_3[0].second;
			error_3.erase(error_3.begin());
			bool num1_success = robots[error_robot_id_1].solve_error(robots[error_robot_id_2].pos);
			if (num1_success)
			{
				count++;
				continue;
			}
			else
			{
				count++;
				bool num2_success = robots[error_robot_id_2].solve_error(robots[error_robot_id_1].pos);
			}
			error_no++;
		}
		get_next_pos();
		error_1 = findAllPairDuplicates(next_point_for_Robots);
		error_2 = find_equal_pairs(now_pos, next_point_for_Robots);
		error_3 = check_error_for_berth(now_pos, next_point_for_Robots);
		error = merge_vectors(error_1, error_2);
		for (int i = 0; i < error.size(); i++)
		{
			int error_robot_id_1 = min(error[0].first, error[0].second);
			int error_robot_id_2 = max(error[0].first, error[0].second);
			if (error_robot_id_1 == error_robot_id_2 || next_point_for_Robots[error_robot_id_2].first == -1 || next_point_for_Robots[error_robot_id_2].second == -1 || next_point_for_Robots[error_robot_id_1].first == -1 || next_point_for_Robots[error_robot_id_1].second == -1)
			{
				del.push_back(i);
			}
		}
		removeIndices_for_4(error, del);
		del.clear();
	}
	error_1.clear();
	error_2.clear();
	error_3.clear();
	error.clear();
	for (int i = 0; i < next_point_for_Robots.size(); i++)
	{
		next_point_for_Robots[i] = make_pair(-1, -1);
	}
}

vector<pair<int, int>> Solver::check_error_for_berth(vector<pair<int, int>> pos, vector<pair<int, int>> next_pos)
{
	vector<int> tmp;
	for (int i = 0; i < next_pos.size(); i++)
	{
		if (next_pos[i].first == -1 && next_pos[i].second == -1)
		{
			tmp.push_back(i);
		}
	}
	vector<pair<int, int>>ans;
	for (int i = 0; i < tmp.size(); i++)
	{
		pair<int, int> tmp_pos = pos[tmp[i]];
		for (int j = 0; j < next_pos.size(); j++)
		{
			if (next_pos[j].first == tmp_pos.first && next_pos[j].second == tmp_pos.second)
			{
				ans.push_back(make_pair(tmp[i], j));
			}
		}
	}
	return ans;
}

void Solver::get_next_pos()
{
	pair<int, int> next_pos;
	for (int i = 0; i < robot_num; i++)
	{
		if (robots[i].path.empty())
		{
			next_pos = make_pair(-1, -1);
		}
		else if (robots[i].path[0] == 0)
		{
			next_pos = make_pair(robots[i].pos.first, robots[i].pos.second + 1);
		}
		else if (robots[i].path[0] == 1)
		{
			next_pos = make_pair(robots[i].pos.first, robots[i].pos.second - 1);
		}
		else if (robots[i].path[0] == 2)
		{
			next_pos = make_pair(robots[i].pos.first - 1, robots[i].pos.second);
		}
		else if (robots[i].path[0] == 3)
		{
			next_pos = make_pair(robots[i].pos.first + 1, robots[i].pos.second);
		}
		next_point_for_Robots[i] = next_pos;
	}
}

pair<int, int> Solver::getBerthPos(int berth_id_pos)
{
	pair<int, int> tmp;
	for (int i = 0; i < 16; i += 5)
	{
		tmp = berths[berth_id_pos].poses[i];
		if (!ifHere(berths[berth_id_pos].has_poses, tmp))
		{
			berths[berth_id_pos].has_poses.push_back(tmp);
			return tmp;
		}
	}
	tmp = berths[berth_id_pos].poses[3];
	if (!ifHere(berths[berth_id_pos].has_poses, tmp))
	{
		berths[berth_id_pos].has_poses.push_back(tmp);
		return tmp;
	}
	tmp = berths[berth_id_pos].poses[12];
	if (!ifHere(berths[berth_id_pos].has_poses, tmp))
	{
		berths[berth_id_pos].has_poses.push_back(tmp);
		return tmp;
	}
	for (int i = 0; i < 16; i++)
	{
		tmp = berths[berth_id_pos].poses[i];
		if (!ifHere(berths[berth_id_pos].has_poses, tmp))
		{
			berths[berth_id_pos].has_poses.push_back(tmp);
			return tmp;
		}
	}
	return tmp;
}

bool Solver::ifHere(const vector<pair<int, int>>& here, const pair<int, int>& goal)
{
	for (int i = 0; i < here.size(); i++)
	{
		if (here[i].first == goal.first && here[i].second == goal.second)
		{
			return true;
		}
	}
	return false;
}

void Solver::get_berths()
{
	//划分区域
	parts = partitionGround(ground);
	//按照面积大小排序
	sort(parts.begin(), parts.end(), [](const vector<pair<int, int>>& a, const vector<pair<int, int>>& b) {
		return a.size() > b.size();
		});
	vector<vector<int>> berths_all;
	vector<vector<int>> robots_all;
	vector<int> del;
	//删除没有港口或者没有机器人的板块，该板块不管
	for (int i = 0; i < parts.size(); i++)
	{
		bool if_del = true;
		for (int j = 0; j < berth_num; j++)
		{
			pair<int, int> pos_tmp = berths[j].pos;
			for (int k = 0; k < parts[i].size(); k++)
			{
				if (parts[i][k].first == pos_tmp.first && parts[i][k].second == pos_tmp.second)
				{
					if_del = false;
					break;
				}
			}
			if (!if_del)
				break;
		}
		if (!if_del)//判断机器人
		{
			for (int j = 0; j < robot_num; j++)
			{
				pair<int, int> pos_tmp = robots[j].pos;
				for (int k = 0; k < parts[i].size(); k++)
				{
					if (parts[i][k].first == pos_tmp.first && parts[i][k].second == pos_tmp.second)
					{
						if_del = false;
						break;
					}
				}
				if (!if_del)
					break;
			}
		}
		if (if_del)
		{
			del.push_back(i);
		}
	}
	removeIndices(parts, del);
	del.clear();
	//每个板块的机器人和港口编号存入数组
	for (int i = 0; i < parts.size(); i++)
	{
		vector<int> berths_tmp;
		vector<int> robots_tmp;
		for (int j = 0; j < berth_num; j++)
		{
			pair<int, int> pos_tmp = berths[j].pos;
			for (int k = 0; k < parts[i].size(); k++)
			{
				if (parts[i][k].first == pos_tmp.first && parts[i][k].second == pos_tmp.second)
				{
					berths_tmp.push_back(j);
					break;
				}
			}
		}
		for (int j = 0; j < robot_num; j++)
		{
			pair<int, int> pos_tmp = robots[j].pos;
			for (int k = 0; k < parts[i].size(); k++)
			{
				if (parts[i][k].first == pos_tmp.first && parts[i][k].second == pos_tmp.second)
				{
					robots_tmp.push_back(j);
					break;
				}
			}
		}
		berths_all.push_back(berths_tmp);
		robots_all.push_back(robots_tmp);
	}

	////删除不能到的港口
	//for (int i = 0; i < berths_all.size(); i++)
	//{
	//	vector<int> ber_tmp = berths_all[i];
	//	vector<int> rob_tmp = robots_all[i];
	//	for (int j = 0; j < ber_tmp.size(); j++)
	//	{
	//		int if_has_robot = false;//是否有机器人能到港口
	//		for (int k = 0; k < rob_tmp.size(); k++)
	//		{
	//			vector<int> path_tmp = findShortestPath(ground, berths[ber_tmp[j]].pos, A_positions[rob_tmp[k]]);
	//			if (path_tmp.size() > 0)
	//			{
	//				if_has_robot = true;
	//				break;
	//			}
	//		}
	//		if (!if_has_robot)
	//		{
	//			del.push_back(j);
	//		}
	//	}
	//	removeIndices_for_3(berths_all[i], del);
	//	del.clear();
	//	sort(berths_all[i].begin(), berths_all[i].end(), [this](const int& a, const int& b) {
	//		return tool_get_berths(a, b);
	//		});
	//}

	////删除不能到的机器人
	//for (int i = 0; i < robots_all.size(); i++)
	//{
	//	vector<int> ber_tmp = berths_all[i];
	//	vector<int> rob_tmp = robots_all[i];
	//	for (int j = 0; j < rob_tmp.size(); j++)
	//	{
	//		int if_has_berth = false;//是否有机器人能到港口
	//		for (int k = 0; k < ber_tmp.size(); k++)
	//		{
	//			vector<int> path_tmp = findShortestPath(ground, berths[ber_tmp[j]].pos, A_positions[rob_tmp[k]]);
	//			if (path_tmp.size() > 0)
	//			{
	//				if_has_berth = true;
	//				break;
	//			}
	//		}
	//		if (!if_has_berth)
	//		{
	//			del.push_back(j);
	//		}
	//	}
	//	removeIndices_for_3(robots_all[i], del);
	//	del.clear();
	//}

	for (int i = 0; i < berths_all.size(); i++)
	{
		sort(berths_all[i].begin(), berths_all[i].end(), [this](const int& a, const int& b) {
			return tool_get_berths(a, b);
			});
	}


	//机器人和港口匹配
	for (int i = 0; i < parts.size(); i++)
	{
		vector<int> robots_of_i(robots_all[i]);
		vector<int> berths_of_i(berths_all[i]);
		int length = min(robots_of_i.size(), berths_of_i.size());
		for (int j = 0; j < length; j++)
		{
			int r_id = robots_of_i[j];
			int b_id = berths_of_i[j];
			robots[r_id].berth_id = berths[b_id].ID;
			robots[r_id].berth_pos = getBerthPos(b_id);
			berths[b_id].RobotIdS.push_back(r_id);
		}
		if (robots_of_i.size() > length)
		{
			for (int j = length, k = 0; j < robots_of_i.size(); j++, k++)
			{
				k = k % length;
				int r_id = robots_of_i[j];
				int b_id = berths_of_i[k];
				robots[r_id].berth_id = berths[b_id].ID;
				robots[r_id].berth_pos = getBerthPos(b_id);
				berths[b_id].RobotIdS.push_back(r_id);
			}
		}
	}

	//储存现在的港口
	for (int i = 0; i < berth_num; i++)
	{
		if (!berths[i].RobotIdS.empty())
		{
			using_berth.push_back(i);
		}
	}
}

bool Solver::tool_get_berths(const int& a, const int& b)
{
	return berths[a].price < berths[b].price;
}
