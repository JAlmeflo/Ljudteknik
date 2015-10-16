#include "Camera.h"


Camera::Camera()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	Publisher::AddSubscriber(this);
}


Camera::~Camera()
{
	Publisher::Unsubscribe(this);
}

void Camera::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
}

void Camera::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
}

D3DXVECTOR3 Camera::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

D3DXVECTOR3 Camera::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

void Camera::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	//m_rotationZ += 1.0f;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// convert to radians
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	lookAt = position + lookAt;

	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);
}

void Camera::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}

void Camera::Update(bool p_keys[256], float deltaTime)
{
	D3DXVECTOR3 pos = GetPosition(); 
	D3DXVECTOR3 rot = GetRotation();
	D3DXVECTOR3 forward;
	D3DXVECTOR3 right;
	D3DXVECTOR3 up;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	forward.x = 0.0f;
	forward.y = 0.0f;
	forward.z = 1.0f;

	right.x = 1.0f;
	right.y = 0.0f;
	right.z = 0.0f;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);
	D3DXVec3TransformCoord(&forward, &forward, &rotationMatrix);
	D3DXVec3TransformCoord(&right, &right, &rotationMatrix);
	//D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	float sense = 50 * deltaTime;

	// w
	if (p_keys[0x57])
	{
		SetPosition(pos.x + sense * forward.x, pos.y + sense * forward.y, pos.z + sense * forward.z);
		pos = GetPosition();
	}
	//a
	if (p_keys[0x41])
	{
		SetPosition(pos.x - sense * right.x, pos.y - sense * right.y, pos.z - sense * right.z);
		pos = GetPosition();
	}
	// s
	if (p_keys[0x53])
	{
		SetPosition(pos.x - sense * forward.x, pos.y - sense * forward.y, pos.z - sense * forward.z);
		pos = GetPosition();
	}
	// d
	if (p_keys[0x44])
	{
		SetPosition(pos.x + sense * right.x, pos.y + sense * right.y, pos.z + sense * right.z);
		pos = GetPosition();
	}
	// Space
	if (p_keys[VK_SPACE])
	{
		SetPosition(pos.x + sense * up.x, pos.y + sense * up.y, pos.z + sense * up.z);
		pos = GetPosition();
	}
	// Ctrl
	if (p_keys[VK_CONTROL])
	{
		SetPosition(pos.x - sense * up.x, pos.y - sense * up.y, pos.z - sense * up.z);
		pos = GetPosition();
	}
}

void Camera::UpdateMouse(int p_x, int p_y, float deltaTime)
{
	D3DXVECTOR3 rot = GetRotation();
	SetRotation(rot.x - 0.1 * p_y, rot.y + 0.1 * p_x, rot.z);
}