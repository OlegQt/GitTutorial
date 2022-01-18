#include "CBall.h"

CBall::CBall(float x, float y, float d)
{
	this->xPos = x;
	this->yPos = y;
	this->Diameter = d;
	this->Vx = 0.01f;
	this->Vy = Vx / 0.12f;
	this->Vz = 0;

}
CBall::CBall()
{}
CBall::~CBall()
{
}

