#pragma once

#include <D3DX10math.h>

class Light
{
public:
	Light();
	~Light();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetPosition(float, float, float);
	void SetLookAt(float, float, float);
	void Rotate(float);

	D3DXVECTOR4 GetAmbientColor();
	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetPosition();

	void GenerateViewMatrix();
	void GenerateProjectionMatrix(float, float);

	void GetViewMatrix(D3DXMATRIX&);
	void GetProjectionMatrix(D3DXMATRIX&);
private:
	D3DXVECTOR4 m_ambientColor;
	D3DXVECTOR4 m_diffuseColor;
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_lookAt;
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_projectionMatrix;
};