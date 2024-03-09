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
		berths.push_back(Berth(id, pos, time, speed));
	}

	//��ȡ����
	scanf("%d", &boat_capacity);

	char ok[5];
	scanf("%s", ok);

	//�������е�A��λ��
	findAndReplaceA(ground);
	getMatch();
	printf("OK\n");
	fflush(stdout);
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
void Solver::getMatch()
{
	vector<pair<int, int>> B;
	for (int i = 0; i < berth_num; i++)
	{
		B.push_back(berths[i].pos);
	}

	match_tmp = findBijectiveMapping(ground, B, A_positions);
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
		scanf("%d %d", &x, &y, &val);
		int die_time = id + 1000;
		pair<int, int> pos = make_pair(x, y);
		getGood(pos, die_time, val);
	}
}

//�����»���
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
