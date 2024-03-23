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

	// 构造函数
	Good(pair<int, int> pos, int val, int dietime, int BerthId, int dist, double price);
	Good();

	// 重载 < 运算符
	bool operator<(const Good& other) const {
		return price < other.price;
	}

	// 重载 > 运算符
	bool operator>(const Good& other) const {
		return price > other.price;
	}

	// 重载 == 运算符
	bool operator==(const Good& other) const {
		return price == other.price;
	}

	// 重载 != 运算符
	bool operator!=(const Good& other) const {
		return price != other.price;
	}

	// 重载 <= 运算符
	bool operator<=(const Good& other) const {
		return price <= other.price;
	}

	// 重载 >= 运算符
	bool operator>=(const Good& other) const {
		return price >= other.price;
	}
};


class Robot
{
public:
	//不变
	int ID;
	int berth_id;
	pair<int, int> berth_pos;
	vector<vector<char>> ground;

	//每帧改变
	pair<int, int> pos;//位置
	pair<int, int> next_pos;
	int now_dir;
	int pre_error;

	//跟随任务改变
	pair<int, int> goal_pos;
	vector<int> path;
	vector<int> go_path;
	vector<int> back_path;
	//vector<int> future_path;

	//状态
	bool if_has;
	int status;
	bool if_initPath;
	bool if_inBerth;
	bool if_chuchai;

	//构造函数
	Robot(int id, pair<int, int> pos, bool if_has, int status, pair<int, int> goal_pos, int berth_id, pair<int, int> berth_pos, vector<vector<char>> ground);
	Robot();

	//获取回来的路径
	void reversePath();

	//初始化路径去港口
	void initPath(const vector<int>& path);

	//每帧更新函数，灵感来自unity 
	bool update();

	//移动函数
	void move();

	//拿取函数
	void get();

	//放下函数
	void pull();

	//获取任务函数
	//void get_task();

	//出队函数
	int del_path();

	//获取下个位置
	void get_next_pos();

	//解决冲突
	bool solve_error(pair<int, int> other_pos);
};

class Berth
{
public:
	int ID;
	pair<int, int> pos;//位置
	int time;//时间
	int speed;//速度
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
	vector<vector<pair<int, int>>> nearestPort; // 存储最近港口位置
	vector<vector<int>> distance; // 存储到最近港口的距离
	int rows, cols;
	vector<pair<int, int>> directions{ {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

public:
	PortFinder(vector<vector<char>>& map, vector<pair<int, int>>& berths) {
		rows = map.size();
		cols = map[0].size();
		nearestPort.resize(rows, vector<pair<int, int>>(cols, { -1, -1 }));
		distance.resize(rows, vector<int>(cols, INT_MAX)); // 初始化距离为最大值

		for (const pair<int, int>& berth : berths) {
			bfs(map, berth);
		}
	}

	pair<int, int> findNearestPort(pair<int, int> location) {
		return nearestPort[location.first][location.second];
	}

	int findNearestPortDistance(pair<int, int> location) {
		// 返回指定位置到最近港口的距离
		return distance[location.first][location.second];
	}

private:
	void bfs(vector<vector<char>>& map, const pair<int, int>& berth) {
		queue<pair<int, int>> q;
		q.push(berth);
		distance[berth.first][berth.second] = 0; // 港口位置到自己的距离是0

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
						nearestPort[newR][newC] = berth; // 更新最近港口位置
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
	//常量
	int n;//200
	int robot_num;//10
	int berth_num;//10
	int boat_num;//5
	int N;//201

	//输入一次后不变
	int boat_capacity;//容量
	vector<pair<int, int>> A_positions;//机器人的初始位置
	vector<pair<int, int>> match_br;//港口到机器人匹配
	vector<pair<int, int>> match_rb;//机器人到港口匹配
	vector<pair<int, int>> match_tmp;//匹配中间值
	vector<pair<pair<int, int>, int>> A_roubt;//位置到机器人编号匹配
	vector<vector<char>> ground;//地图
	vector<int> using_berth;
	double berths_dist;
	vector<int> berths_busy;
	vector<vector<pair<int, int>>> parts;
	vector<vector<int>> berths_all;
	vector<vector<int>> robots_all;

	//对象
	vector<Robot> robots;
	vector<Berth> berths;
	vector<Boat> boats;
	PortFinder* pf;

	//每帧更改
	int money;
	int id;
	int new_num;
	int error_no;

	vector<pair<int, int>> next_point_for_Robots;
	//状态
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
