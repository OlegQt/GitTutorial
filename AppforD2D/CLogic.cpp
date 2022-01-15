#include "CLogic.h"
CLogic::CLogic()
{
}

CLogic::~CLogic()
{
	this->array.clear();
}

void CLogic::AddElement(float xPos, float yPos, float Diameter)
{
	this->array.push_back(CBall(xPos,yPos));
}
