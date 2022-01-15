#pragma once
#include "CBall.h"
#include <vector>

class CLogic
{
public:
	CLogic();
	~CLogic();

	void AddElement(float,float,float);
	int i = 0;
private:
	
	std::vector<CBall>array;
};

