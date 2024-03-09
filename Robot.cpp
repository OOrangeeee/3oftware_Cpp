#include"classes.h"

Robot::Robot(int id, pair<int, int> pos, bool if_has, int status, pair<int, int> goal_pos, int berth_id, pair<int, int> berth_pos)
{
	this->ID = id;
	this->pos = pos;
	this->if_has = if_has;
	this->status = status;
	this->goal_pos = goal_pos;
	this->berth_id = berth_id;
	this->berth_pos = berth_pos;
}