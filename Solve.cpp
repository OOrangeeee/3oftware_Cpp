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
	if_getMatch = false;
}

//��ʼ������
void Solver::init()
{
	//��ȡ��ͼ
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

	//��ʼ��������
	for (int i = 0; i < robot_num; i++)
	{
		robots[i] = Robot(i, make_pair(-1, -1), false, 1, make_pair(-1, -1), -1, make_pair(-1, -1), ground);
	}

	//��ʼ����
	for (int i = 0; i < boat_num; i++)
	{
		boats[i] = Boat(-1, -1);
	}

	//��ȡ�ۿ�
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

	//��ȡ����
	scanf("%d", &boat_capacity);

	for (int i = 0; i < boat_num; i++)
	{
		boats[i].can_have = boat_capacity;
		boats[i].id = i;
	}

	get_Boat_Berth_match();

	char ok[5];
	scanf("%s", ok);

	//�������е�A��λ��
	findAndReplaceA(ground);
	getMatchTmp();
	printf("OK\n");
	fflush(stdout);
}

//ÿһ֡������
void Solver::everyInput()
{
	//��ȡ֡����Ǯ��
	scanf("%d %d", &id, &money);

	//��ȡ�»�����
	scanf("%d", &new_num);

	//��ȡ�»�����Ϣ
	for (int i = 0; i < new_num; i++)
	{
		int x, y, val;
		scanf("%d %d %d", &x, &y, &val);
		int die_time = id + 1000;
		pair<int, int> pos = make_pair(x, y);
		getGood(pos, die_time, val);
	}

	//��ȡ��������Ϣ
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

	//���ܶ�Ӧ�ĸۿںͻ����˶�Ӧ����
	if (!if_getMatch)
	{
		get_match();
		//���ܶ�Ӧ�Ļ����˺͸ۿڱ�������
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

	//��ȡ����Ϣ
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
	//��������
	for (int i = 0; i < berth_num; i++)
	{
		if (berths[i].RobotId != -1)
		{
			if (!berths[i].Good_future.empty() && robots[berths[i].RobotId].future_path.empty() && robots[berths[i].RobotId].if_inBerth)
			{
				vector<int> tmp_path = berths[i].give_task(id, ground);
				if (!tmp_path.empty())
				{
					robots[berths[i].RobotId].future_path = tmp_path;
				}
			}
		}
	}

	//ָʾ������
	for (int i = 0; i < robot_num; i++)
	{
		if (robots[i].berth_id != -1)//���õĻ�����
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
					berths[robots[i].berth_id].count++;
				}
			}
		}
	}

	//ָʾ��
	for (int i = 0; i < boat_num; i++)
	{
		boats[i].zhen = id;
		boats[i].berthId_1_num = berths[boats[i].berthId_1].count;
		boats[i].berthId_2_num = berths[boats[i].berthId_2].count;
		if (boats[i].status == 1 || boats[i].status == 2)
		{
			boats[i].update();
		}
	}

}

void Solver::get_match()
{
	for (int i = 0; i < match_tmp.size(); i++)
	{
		int index = match_tmp[i].second;
		pair<int, int> tmp_pos = A_positions[index];
		for (int j = 0; j < A_roubt.size(); j++)
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

//�ҵ�����A��λ�ã������µ�ͼ
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

//��ÿ���ۿھ�����ƥ��һ��������,��ʱ�õ����Ǹۿ�ID�����л�����λ�õ������Ķ�Ӧ
void Solver::getMatchTmp()
{
	vector<pair<int, int>> B;
	for (int i = 0; i < berth_num; i++)
	{
		B.push_back(berths[i].pos);
	}

	match_tmp = findBijectiveMapping(ground, B, A_positions);//�ۿ�id ������λ������
}



//�����»���
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

void Solver::get_Boat_Berth_match()
{
	vector<int> time_berth;
	int x = 0;
	for (int i = 0; i < berth_num; i++)
	{
		time_berth.push_back(berths[i].time);
	}
	sort(time_berth.begin(), time_berth.end());
	for (int i = 0; i < berth_num / 2; i++, x++)
	{
		int num1 = time_berth[i];
		int num2 = time_berth[berth_num - i - 1];
		int index1 = 0;
		int index2 = 0;
		for (int j = 0; j < berth_num; j++)
		{
			if (berths[j].time == num1 && berths[j].BoatId == -1)
			{
				berths[j].BoatId = x;
				index1 = j;
				break;
			}
		}
		for (int j = 0; j < berth_num; j++)
		{
			if (berths[j].time == num2 && berths[j].BoatId == -1)
			{
				berths[j].BoatId = x;
				index2 = j;
				break;
			}
		}
		if (berths[index1].speed > berths[index2].speed)
		{
			boats[i].berthId_1 = index1;
			boats[i].berthId_2 = index2;
			boats[i].berthId_1_speed = berths[index1].speed;
			boats[i].berthId_2_speed = berths[index2].speed;
			boats[i].berthId_1_time = berths[index1].time;
			boats[i].berthId_2_time = berths[index2].time;
		}
		else
		{
			boats[i].berthId_1 = index2;
			boats[i].berthId_2 = index1;
			boats[i].berthId_1_speed = berths[index2].speed;
			boats[i].berthId_2_speed = berths[index1].speed;
			boats[i].berthId_1_time = berths[index2].time;
			boats[i].berthId_2_time = berths[index1].time;
		}
	}
}
