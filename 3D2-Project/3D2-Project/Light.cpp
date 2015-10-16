#include "Light.h"


Light::Light()
{
}


Light::~Light()
{
}

void Light::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = D3DXVECTOR4(red, green, blue, alpha);
}

void Light::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = D3DXVECTOR4(red, green, blue, alpha);
}

void Light::SetPosition(float x, float y, float z)
{
	m_position = D3DXVECTOR3(x, y, z);
}

void Light::SetLookAt(float x, float y, float z)
{
	m_lookAt = D3DXVECTOR3(x, y, z);
}

void Light::Rotate(float rotation)
{
	float x = cos(rotation);
	float y = sin(rotation);
	
	SetPosition(x * 50.0f, GetPosition().y, y * 50.0f);
}

D3DXVECTOR4 Light::GetAmbientColor()
{
	return m_ambientColor;
}

D3DXVECTOR4 Light::GetDiffuseColor()
{
	return m_diffuseColor;
}

D3DXVECTOR3 Light::GetPosition()
{
	return m_position;
}

void Light::GenerateViewMatrix()
{
	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);

	D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &m_lookAt, &up);
}

void Light::GenerateProjectionMatrix(float screenDepth, float screenNear)
{
	float fieldOfView, screenAspect;

	fieldOfView = D3DX_PI * 0.5f;
	screenAspect = 1.0f;

	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);
}

void Light::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}

void Light::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
}