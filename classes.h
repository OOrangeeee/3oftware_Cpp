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
	void initPath(vector<int> path);

	//每帧更新函数，灵感来自unity 
	void update();

	//移动函数
	void move();

	//拿取函数
	void get();

	//放下函数
	void pull();

	//获取任务函数
	void get_task();

	//出队函数
	void del_path();
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
	int berthId;

	Boat(int status, int goal);
	Boat();
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

};
