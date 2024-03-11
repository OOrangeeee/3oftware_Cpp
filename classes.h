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
	void initPath(vector<int> path);

	//ÿ֡���º������������unity 
	void update();

	//�ƶ�����
	void move();

	//��ȡ����
	void get();

	//���º���
	void pull();

	//��ȡ������
	void get_task();

	//���Ӻ���
	void del_path();
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
	int berthId;

	Boat(int status, int goal);
	Boat();
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

};
