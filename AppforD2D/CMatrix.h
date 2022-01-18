#pragma once
#include <windows.h>
#include <iostream>
#include <cmath>
#define PI 3.14159265

class CMatrix
{
	CMatrix();
	~CMatrix();
public:
	static std::pair<float, float> rotate(float,float);
};

