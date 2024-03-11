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
	this->count = 0;
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
	this->count = 0;
	this->id = -1;
	this->zhen = -1;
	this->if_ship = 0;
}

void Boat::update()
{
	if_ship++;
	if (goal == -1)
	{
		ship(berthId_1);
		return;
	}
	if (if_ship % 500 != 0)
		return;
	if (goal == berthId_1)
	{
		if (zhen + berthId_1_time + 40 >= 300)
		{
			go();
			return;
		}
		count += min(berthId_1_speed, berthId_1_num);
		ship(berthId_1);
		return;
	}
	else if (goal == berthId_2)
	{
		if (zhen + berthId_2_time + 40 >= 300)
		{
			go();
			return;
		}
		count += min(berthId_2_speed, berthId_2_num);
		ship(berthId_2);
		return;
	}
	if (count > can_have)
	{
		go();
		return;
	}
	if (count + berthId_2_num >= can_have)
	{
		ship(berthId_2);
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
}