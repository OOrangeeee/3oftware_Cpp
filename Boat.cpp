#include"classes.h"

Boat::Boat(int status, int goal)
{
	this->status = status;
	this->goal = goal;
	this->berthId_1 = -1;
	this->berthId_2 = -1;
	this->if_pre_berth = true;
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
	this->if_ship = 0;
}

Boat::Boat()
{
	this->status = -1;
	this->goal = -1;
	this->berthId_1 = -1;
	this->berthId_2 = -1;
	this->if_pre_berth = true;
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
	this->if_ship = 0;
}

void Boat::update()
{
	/*if_ship++;
	if (if_ship % 500 == 0)
		return;
	if (status == 1 || goal == berthId_1)
	{
		counts += min(berthId_1_num, berthId_1_speed);
	}
	else if (status == 1 || goal == berthId_2)
	{
		counts += min(berthId_2_num, berthId_2_speed);
	}
	if (counts >= can_have)
	{
		go();
		return;
	}
	if (zhen > 11000)
	{
		go();
		return;
	}*/
	
		go();
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