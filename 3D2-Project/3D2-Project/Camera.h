#pragma once

#include <D3DX10math.h>
#include "Subscriber.h"
#include "Publisher.h"
#include <iostream>

class Camera : public Subscriber
{
public:
	Camera();
	~Camera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX&);

	void Update(bool p_keys[256], float deltaTime);
	void UpdateMouse(int p_x, int p_y, float deltaTime);
private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;
};

