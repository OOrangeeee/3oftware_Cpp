#include"classes.h"

Boat::Boat(int status, int goal)
{
	this->status = status;
	this->goal = goal;
}

Boat::Boat()
{
	this->status = -1;
	this->goal = -1;
}