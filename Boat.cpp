#include"classes.h"

Boat::Boat(int status, int goal)
{
	this->status = status;
	this->goal = goal;
	this->berthId_1 = -1;
	this->can_have = -1;
	this->berthId_1_num = 0;
	this->berthId_1_speed = 0;
	this->berthId_1_time = -1;
	this->counts = 0;
	this->id = -1;
	this->zhen = -1;
}

Boat::Boat()
{
	this->status = -1;
	this->goal = -1;
	this->berthId_1 = -1;
	this->can_have = -1;
	this->berthId_1_num = 0;
	this->berthId_1_speed = 0;
	this->berthId_1_time = -1;
	this->counts = 0;
	this->id = -1;
	this->zhen = -1;
}

void Boat::update()
{
	if (status == 1 && goal == berthId_1)
	{
		int to_add = min(berthId_1_num, berthId_1_speed);
		counts += to_add;
		berthId_1_num -= to_add;
		if (zhen + berthId_1_time + 10 > 15000)
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