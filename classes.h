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

	// 构造函数
	Good(pair<int, int> pos, int val, int dietime, int BerthId, int dist, int price);
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

	//跟随任务改变
	pair<int, int> goal_pos;
	vector<int> path;
	vector<int> go_path;
	vector<int> back_path;
	vector<int> future_path;

	//状态
	bool if_has;
	int status;
	bool if_initPath;
	bool if_inBerth;

	//构造函数
	Robot(int id, pair<int, int> pos, bool if_has, int status, pair<int, int> goal_pos, int berth_id, pair<int, int> berth_pos, vector<vector<char>> ground);
	Robot();

	//获取回来的路径
	void resverPath();

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
	void get_task();

	//出队函数
	int del_path();

	//获取下个位置
	void get_next_pos();

	//解决冲突
	bool solve_error();
};

class Berth
{
public:
	int ID;
	pair<int, int> pos;//位置
	int time;//时间
	int speed;//速度
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
	//对象
	vector<Robot> robots;
	vector<Berth> berths;
	vector<Boat> boats;
	//每帧更改
	int money;
	int id;
	int new_num;
	vector<pair<int,int>> next_point_for_Robots;
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
};
