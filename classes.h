#define _CRT_SECURE_NO_WARNINGS
#include"tools.h"
using namespace std;

class Good {
public:
	pair<int, int> pos;
	int value;
	int dietime;
	int BerthId;
	int dist;
	int price;

	// ���캯��
	Good(pair<int, int> pos, int val, int dietime, int BerthId, int dist, int price);
	Good();

	// ���� < �����
	bool operator<(const Good& other) const {
		return price < other.price;
	}

	// ���� > �����
	bool operator>(const Good& other) const {
		return price > other.price;
	}

	// ���� == �����
	bool operator==(const Good& other) const {
		return price == other.price;
	}

	// ���� != �����
	bool operator!=(const Good& other) const {
		return price != other.price;
	}

	// ���� <= �����
	bool operator<=(const Good& other) const {
		return price <= other.price;
	}

	// ���� >= �����
	bool operator>=(const Good& other) const {
		return price >= other.price;
	}
};


class Robot
{
public:
	//����
	int ID;
	int berth_id;
	pair<int, int> berth_pos;
	vector<vector<char>> ground;

	//ÿ֡�ı�
	pair<int, int> pos;//λ��
	pair<int, int> next_pos;
	int now_dir;

	//��������ı�
	pair<int, int> goal_pos;
	vector<int> path;
	vector<int> go_path;
	vector<int> back_path;
	vector<int> future_path;

	//״̬
	bool if_has;
	int status;
	bool if_initPath;
	bool if_inBerth;

	//���캯��
	Robot(int id, pair<int, int> pos, bool if_has, int status, pair<int, int> goal_pos, int berth_id, pair<int, int> berth_pos, vector<vector<char>> ground);
	Robot();

	//��ȡ������·��
	void resverPath();

	//��ʼ��·��ȥ�ۿ�
	void initPath(const vector<int>& path);

	//ÿ֡���º������������unity 
	bool update();

	//�ƶ�����
	void move();

	//��ȡ����
	void get();

	//���º���
	void pull();

	//��ȡ������
	void get_task();

	//���Ӻ���
	int del_path();

	//��ȡ�¸�λ��
	void get_next_pos();

	//�����ͻ
	bool solve_error();
};

class Berth
{
public:
	int ID;
	pair<int, int> pos;//λ��
	int time;//ʱ��
	int speed;//�ٶ�
	int RobotId;
	int BoatId;
	SortedList<Good> Good_future;

	Berth(int id, pair<int, int> pos, int time, int speed);
	Berth();

	vector<int> give_task(int ID, vector<vector<char>> map);
};

class Boat
{
public:
	int goal;
	int status;
	int berthId_1;
	int berthId_2;
	int berthId_1_num;
	int berthId_2_num;
	int berthId_1_time;
	int berthId_2_time;
	int berthId_1_speed;
	int berthId_2_speed;
	int counts;
	int can_have;
	int id;
	int zhen;

	Boat(int status, int goal);
	Boat();
	void update();
	void ship(int goal);
	void go();
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
	vector<pair<int,int>> next_point_for_Robots;
	//״̬
	bool if_getMatch;

	Solver();

	void init();

	void findAndReplaceA(vector<vector<char>>& ground);

	void getMatchTmp();

	void everyInput();

	void getGood(pair<int, int> pos, int die_time, int val);

	void get_match();

	void action();

	void get_Boat_Berth_match();

	void check_error();

	void get_next_pos();
};
