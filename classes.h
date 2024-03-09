#include <vector>
#include <map>
#include"tools.h"

class Robot
{
public:
	//不变
	int ID;
	int berth_id;
	pair<int, int> berth_pos;

	//每帧改变
	pair<int, int> pos;//位置
	bool if_has;
	int status;

	//跟随任务改变
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
	pair<int, int> pos;//位置
	int time;//时间
	int speed;//速度
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

	// 重载 < 运算符
	bool operator<(const Good& other) const {
		return price < other.price;
	}

	// 重载 > 运算符
	bool operator>(const Good& other) const {
		return price > other.price;
	}

	Good(pair<int, int> pos, int val, int dietime, int BerthId, int dist, int price);
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
	vector<int> ground;//地图
	vector<pair<int, int>> A_positions;//机器人的初始位置
	vector<pair<int, int>> match_br;//港口到机器人匹配
	vector<pair<int, int>> match_rb;//机器人到港口匹配
	vector<pair<int, int>> match_tmp;//匹配中间值
	vector<pair<pair<int, int>, int>> A_roubt;//位置到机器人编号匹配
	vector<vector<char>> ground;//地图
	//对象
	vector<Robot> robots;
	vector<Berth> berths;
	vector<Boat> boats;
	//每帧更改
	int money;
	int id;
	int new_num;
	//状态
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
