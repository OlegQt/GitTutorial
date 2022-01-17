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
void CLogic::SolveInteraction(int a, int b)
{
	CBall* ballA = this->GetElement(a);
	CBall* ballB = this->GetElement(b);
	// полная скорость шара А
	//float vA = 
	// расчет косинуса угла между вектором скорости первого шара и осью между ними



}
unsigned int CLogic::GetArraySize()
{
	return this->array.size();
}
CBall* CLogic::GetElement(int num)
{
	return &this->array.at(num);
}
