#include"classes.h"

Good::Good(pair<int,int> pos,int val,int dietime,int BerthId,int dist,double price)
{
	this->pos = pos;
	this->value = val;
	this->dietime = dietime;
	this->BerthId = BerthId;
	this->dist = dist;
	this->price = price;
}

Good::Good()
{
	this->pos = make_pair(-1, -1);
	this->value = -1;
	this->dietime = -1;
	this->BerthId = -1;
	this->dist = -1;
	this->price = -1;
}