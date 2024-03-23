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
	double price;

	// ���캯��
	Good(pair<int, int> pos, int val, int dietime, int BerthId, int dist, double price);
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
	int pre_error;

	//��������ı�
	pair<int, int> goal_pos;
	vector<int> path;
	vector<int> go_path;
	vector<int> back_path;
	//vector<int> future_path;

	//״̬
	bool if_has;
	int status;
	bool if_initPath;
	bool if_inBerth;
	bool if_chuchai;

	//���캯��
	Robot(int id, pair<int, int> pos, bool if_has, int status, pair<int, int> goal_pos, int berth_id, pair<int, int> berth_pos, vector<vector<char>> ground);
	Robot();

	//��ȡ������·��
	void reversePath();

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
	//void get_task();

	//���Ӻ���
	int del_path();

	//��ȡ�¸�λ��
	void get_next_pos();

	//�����ͻ
	bool solve_error(pair<int, int> other_pos);
};

class Berth
{
public:
	int ID;
	pair<int, int> pos;//λ��
	int time;//ʱ��
	int speed;//�ٶ�
	vector<int> RobotIdS;
	int BoatId;
	SortedList<Good> Good_future;
	vector<pair<int, int>> poses;
	vector<pair<int, int>> has_poses;
	double price;
	int part;

	Berth(int id, pair<int, int> pos, int time, int speed);
	Berth();

	vector<int> give_task(int ID, vector<vector<char>> map, pair<int, int> now_pos_berth);
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

class PortFinder {
private:
	vector<vector<pair<int, int>>> nearestPort; // �洢����ۿ�λ��
	vector<vector<int>> distance; // �洢������ۿڵľ���
	int rows, cols;
	vector<pair<int, int>> directions{ {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

public:
	PortFinder(vector<vector<char>>& map, vector<pair<int, int>>& berths) {
		rows = map.size();
		cols = map[0].size();
		nearestPort.resize(rows, vector<pair<int, int>>(cols, { -1, -1 }));
		distance.resize(rows, vector<int>(cols, INT_MAX)); // ��ʼ������Ϊ���ֵ

		for (const pair<int, int>& berth : berths) {
			bfs(map, berth);
		}
	}

	pair<int, int> findNearestPort(pair<int, int> location) {
		return nearestPort[location.first][location.second];
	}

	int findNearestPortDistance(pair<int, int> location) {
		// ����ָ��λ�õ�����ۿڵľ���
		return distance[location.first][location.second];
	}

private:
	void bfs(vector<vector<char>>& map, const pair<int, int>& berth) {
		queue<pair<int, int>> q;
		q.push(berth);
		distance[berth.first][berth.second] = 0; // �ۿ�λ�õ��Լ��ľ�����0

		while (!q.empty()) {
			pair<int, int> current = q.front();
			q.pop();
			for (const pair<int, int>& dir : directions) {
				int newR = current.first + dir.first;
				int newC = current.second + dir.second;

				if (newR >= 0 && newR < rows && newC >= 0 && newC < cols &&
					(map[newR][newC] == '.' || map[newR][newC] == 'B')) {
					if (distance[newR][newC] > distance[current.first][current.second] + 1) {
						distance[newR][newC] = distance[current.first][current.second] + 1;
						nearestPort[newR][newC] = berth; // ��������ۿ�λ��
						q.push(make_pair(newR, newC));
					}
				}
			}
		}
	}
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
	vector<int> using_berth;
	double berths_dist;
	vector<int> berths_busy;
	vector<vector<pair<int, int>>> parts;
	vector<vector<int>> berths_all;
	vector<vector<int>> robots_all;

	//����
	vector<Robot> robots;
	vector<Berth> berths;
	vector<Boat> boats;
	PortFinder* pf;

	//ÿ֡����
	int money;
	int id;
	int new_num;
	int error_no;

	vector<pair<int, int>> next_point_for_Robots;
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

	pair<int, int> getBerthPos(int berth_id_pos);

	bool ifHere(const vector<pair<int, int>>& here, const pair<int, int>& goal);

	vector<pair<int, int>> check_error_for_berth(vector<pair<int, int>> pos, vector<pair<int, int>> next_pos);

	void get_berths();

	bool tool_get_berths(const int& a, const int& b);
};
