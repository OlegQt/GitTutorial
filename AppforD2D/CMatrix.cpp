#include "CMatrix.h"
CMatrix::CMatrix(){}
CMatrix::~CMatrix(){}


std::pair<float, float> CMatrix::rotate(float x,float y)
{
	float x2, y2;
	float omega = (1.00f * PI)/180.0f;
	x2 = x * cos(omega) - y * sin(omega);
	y2 = x * sin(omega) + y * cos(omega);
	return std::pair<float, float>(x2,y2);
}
