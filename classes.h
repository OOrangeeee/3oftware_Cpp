#include <vector>
#include <map>
#include"tools.h"

class Robot
{
public:
	//����
	int ID;
	int berth_id;
	pair<int, int> berth_pos;

	//ÿ֡�ı�
	pair<int, int> pos;//λ��
	bool if_has;
	int status;

	//��������ı�
	pair<int, int> goal_pos;
	vector<int> path;
	vector<int> go_path;
	vector<int> back_path;

	Robot(int id, pair<int, int> pos, bool if_has, int status, pair<int, int> goal_pos, int berth_id, pair<int, int> berth_pos);
};

class Berth
{
public:
	int ID;
	pair<int, int> pos;//λ��
	int time;//ʱ��
	int speed;//�ٶ�
	int RobotId;
	SortedList<Good> Good_future;

	Berth(int id, pair<int, int> pos, int time, int speed);
};

class Boat
{
public:
	int goal;
	int status;

	Boat(int status, int goal);
};

class Good
{
public:
	pair<int, int> pos;
	int value;
	int dietime;
	int BerthId;
	int dist;
	int price;

	// ���� < �����
	bool operator<(const Good& other) const {
		return price < other.price;
	}

	// ���� > �����
	bool operator>(const Good& other) const {
		return price > other.price;
	}

	Good(pair<int, int> pos, int val, int dietime, int BerthId, int dist, int price);
};

class Solver
{
public:
	//����
	int n;//200
	int robot_num;//10
	int berth_num;//10
	int boat_num;//5
	int N;//201

	//����һ�κ󲻱�
	int boat_capacity;//����
	vector<int> ground;//��ͼ
	vector<pair<int, int>> A_positions;//�����˵ĳ�ʼλ��
	vector<pair<int, int>> match_br;//�ۿڵ�������ƥ��
	vector<pair<int, int>> match_rb;//�����˵��ۿ�ƥ��
	vector<pair<int, int>> match_tmp;//ƥ���м�ֵ
	vector<pair<pair<int, int>, int>> A_roubt;//λ�õ������˱��ƥ��
	vector<vector<char>> ground;//��ͼ
	//����
	vector<Robot> robots;
	vector<Berth> berths;
	vector<Boat> boats;
	//ÿ֡����
	int money;
	int id;
	int new_num;
	//״̬
	bool if_getMatch;

	Solver();

	void init();

	void findAndReplaceA(vector<vector<char>>& ground);

	void getMatch();

	void everyInput();

	void getGood(pair<int, int> pos, int die_time, int val);

	void get_match();

	void success();

};
