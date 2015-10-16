#pragma once

#include "Publisher.h"

class Input
{
public:
	Input();
	~Input();

	bool Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

	void Update(float deltaTime);
	void UpdateMouse(int p_x, int p_y, float deltaTime);
private:
	bool m_keys[256];
	bool m_firstUpdate;
	Publisher m_publisher;
};

