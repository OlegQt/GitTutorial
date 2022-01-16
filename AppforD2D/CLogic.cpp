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
	this->array.push_back(CBall(xPos,yPos,Diameter));
}

unsigned int CLogic::GetArraySize()
{
	return this->array.size();
}

CBall* CLogic::GetElement(int num)
{
	return &this->array.at(num);
}
