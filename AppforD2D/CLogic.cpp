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
	float x = CMatrix::rotate(ballA->xPos, ballA->yPos).first;
	float y = CMatrix::rotate(ballA->xPos, ballA->yPos).second;
	ballA->xPos = x;
	ballA->yPos = y;
	// ������ �������� ���� �
	//float vA = 
	// ������ �������� ���� ����� �������� �������� ������� ���� � ���� ����� ����



}
unsigned int CLogic::GetArraySize()
{
	return this->array.size();
}
CBall* CLogic::GetElement(int num)
{
	return &this->array.at(num);
}
