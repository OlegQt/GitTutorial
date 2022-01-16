#pragma once
#include "CBall.h"
#include <vector>

class CLogic
{
public:
	CLogic();
	~CLogic();

	void AddElement(float,float,float);
	unsigned int GetArraySize();
	CBall* GetElement(int num);

private:
	
	std::vector<CBall>array;
};

