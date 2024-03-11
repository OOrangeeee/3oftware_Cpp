#include"classes.h"

Boat::Boat(int status, int goal)
{
	this->status = status;
	this->goal = goal;
	this->berthId = -1;
}

Boat::Boat()
{
	this->status = -1;
	this->goal = -1;
	this->berthId = -1;
}