#include "Frustum.h"


Frustum::Frustum()
{
}


Frustum::~Frustum()
{
}

void Frustum::ConstructFrustum(float screenDepth, D3DXMATRIX projectionMatrix, D3DXMATRIX viewMatrix)
{
	float zMin, r;
	D3DXMATRIX matrix;

	zMin = -projectionMatrix._43 / projectionMatrix._33;
	r = screenDepth / (screenDepth - zMin);
	projectionMatrix._33 = r;
	projectionMatrix._43 = -r * zMin;

	D3DXMatrixMultiply(&matrix, &viewMatrix, &projectionMatrix);


	// near
	m_planes[0].a = matrix._14 + matrix._13;
	m_planes[0].b = matrix._24 + matrix._23;
	m_planes[0].c = matrix._34 + matrix._33;
	m_planes[0].d = matrix._44 + matrix._43;
	D3DXPlaneNormalize(&m_planes[0], &m_planes[0]);

	// far
	m_planes[1].a = matrix._14 - matrix._13;
	m_planes[1].b = matrix._24 - matrix._23;
	m_planes[1].c = matrix._34 - matrix._33;
	m_planes[1].d = matrix._44 - matrix._43;
	D3DXPlaneNormalize(&m_planes[1], &m_planes[1]);

	// left
	m_planes[2].a = matrix._14 + matrix._11;
	m_planes[2].b = matrix._24 + matrix._21;
	m_planes[2].c = matrix._34 + matrix._31;
	m_planes[2].d = matrix._44 + matrix._41;
	D3DXPlaneNormalize(&m_planes[2], &m_planes[2]);

	// right
	m_planes[3].a = matrix._14 - matrix._11;
	m_planes[3].b = matrix._24 - matrix._21;
	m_planes[3].c = matrix._34 - matrix._31;
	m_planes[3].d = matrix._44 - matrix._41;
	D3DXPlaneNormalize(&m_planes[3], &m_planes[3]);

	// top
	m_planes[4].a = matrix._14 - matrix._12;
	m_planes[4].b = matrix._24 - matrix._22;
	m_planes[4].c = matrix._34 - matrix._32;
	m_planes[4].d = matrix._44 - matrix._42;
	D3DXPlaneNormalize(&m_planes[4], &m_planes[4]);

	// bottom
	m_planes[5].a = matrix._14 + matrix._12;
	m_planes[5].b = matrix._24 + matrix._22;
	m_planes[5].c = matrix._34 + matrix._32;
	m_planes[5].d = matrix._44 + matrix._42;
	D3DXPlaneNormalize(&m_planes[5], &m_planes[5]);
}

bool Frustum::CheckPoint(float x, float y, float z)
{
	for (int i = 0; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(x, y, z)) < 0.0f)
		{
			return false;
		}
	}
	return true;
}

bool Frustum::CheckSphere(float x, float y, float z, float r)
{
	for (int i = 0; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(x, y, z)) < -r)
		{
			return false;
		}
	}

	return true;
}

bool Frustum::CheckRectangle(float x, float y, float z, float xSize, float ySize, float zSize)
{
	for (int i = 0; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(x - xSize, y - ySize, z - zSize)) >= 0.0f)
		{
			continue;
		}
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(x + xSize, y - ySize, z - zSize)) >= 0.0f)
		{
			continue;
		}
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(x - xSize, y + ySize, z - zSize)) >= 0.0f)
		{
			continue;
		}
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(x + xSize, y + ySize, z - zSize)) >= 0.0f)
		{
			continue;
		}
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(x - xSize, y - ySize, z + zSize)) >= 0.0f)
		{
			continue;
		}
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(x + xSize, y - ySize, z + zSize)) >= 0.0f)
		{
			continue;
		}
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(x - xSize, y + ySize, z + zSize)) >= 0.0f)
		{
			continue;
		}
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(x + xSize, y + ySize, z + zSize)) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}

bool Frustum::CheckRectangle2(float xMin, float yMin, float zMin, float xMax, float yMax, float zMax)
{
	for (int i = 0; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(xMin, yMin, zMin)) >= 0.0f)
		{
			continue;
		}
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(xMax, yMin, zMin)) >= 0.0f)
		{
			continue;
		}
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(xMin, yMax, zMin)) >= 0.0f)
		{
			continue;
		}
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(xMax, yMax, zMin )) >= 0.0f)
		{
			continue;
		}
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(xMin, yMin, zMax)) >= 0.0f)
		{
			continue;
		}
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(xMax, yMin , zMax)) >= 0.0f)
		{
			continue;
		}
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(xMin, yMax, zMax)) >= 0.0f)
		{
			continue;
		}
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(xMax, yMax, zMax)) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}