#include"classes.h"

Berth::Berth(int id,pair<int,int> pos,int time,int speed)
{
	this->ID = id;
	this->pos = pos;
	this->time = time;
	this->speed = speed;
}