#include"classes.h"

Boat::Boat(int status, int goal)
{
	this->status = status;
	this->goal = goal;
	this->berthId_1 = -1;
	this->berthId_2 = -1;
	this->can_have = -1;
	this->berthId_1_num = 0;
	this->berthId_2_num = 0;
	this->berthId_1_speed = 0;
	this->berthId_2_speed = 0;
	this->berthId_1_time = -1;
	this->berthId_2_time = -1;
	this->counts = 0;
	this->id = -1;
	this->zhen = -1;
}

Boat::Boat()
{
	this->status = -1;
	this->goal = -1;
	this->berthId_1 = -1;
	this->berthId_2 = -1;
	this->can_have = -1;
	this->berthId_1_num = 0;
	this->berthId_2_num = 0;
	this->berthId_1_speed = 0;
	this->berthId_2_speed = 0;
	this->berthId_1_time = -1;
	this->berthId_2_time = -1;
	this->counts = 0;
	this->id = -1;
	this->zhen = -1;
}

void Boat::update()
{
	if (status == 1 || goal == berthId_1)
	{
		int to_add = min(berthId_1_num, berthId_1_speed);
		counts += to_add;
		berthId_1_num -= to_add;
		if (zhen + berthId_1_time + 500 + 10 > 15000)
		{
			go();
			return;
		}
	}
	else if (status == 1 || goal == berthId_2)
	{
		int to_add = min(berthId_2_num, berthId_2_speed);
		counts += to_add;
		berthId_2_num -= to_add;
		if (zhen + berthId_2_time + 10 > 15000)
		{
			go();
			return;
		}
	}
	if (counts >= can_have)
	{
		go();
		return;
	}
	if (berthId_1_num == 0)
	{
		ship(berthId_2);
		return;
	}
	if (berthId_2_num + counts > can_have)
	{
		if (500 + berthId_2_time + 10 < 15000)
		{
			int good_need = can_have - counts;
			int time_tmp_2 = good_need / berthId_2_speed + 1 + 500 + berthId_2_time;
			int time_tmp_1 = good_need / berthId_1_speed + 1 + berthId_1_time;
			if (time_tmp_2 < time_tmp_1)
			{
				ship(berthId_2);
				return;
			}

		}
	}
}

void Boat::ship(int goal)
{
	printf("ship %d %d\n", id, goal);
	fflush(stdout);
}

void Boat::go()
{
	printf("go %d\n", id);
	fflush(stdout);
	counts = 0;
}