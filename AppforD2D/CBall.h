#pragma once
#include <windows.h>
class CBall
{
public:
	CBall(float,float,float);
	CBall();
	~CBall();

	float xPos, yPos,Diameter;
	float Vx, Vy, Vz;
	float Mass;

	typedef struct vector
	{
		float pos;
	};
	vector pos = { 0.1f };

};


