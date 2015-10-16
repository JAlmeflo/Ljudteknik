#pragma once

#include <Windows.h>

class Subscriber
{
public:
	Subscriber();
	~Subscriber();

	virtual void Update(bool p_keys[256], float deltaTime) = 0;
	virtual void UpdateMouse(int p_x, int p_y, float deltaTime) = 0;
};

