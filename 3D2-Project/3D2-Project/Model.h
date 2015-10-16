#pragma once

#include <D3D11.h>
#include <D3DX10math.h>
#include <fstream>
#include <iostream>
#include "Texture.h"
#include "OBJReader.h"

using namespace std;

class Model
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct InstanceType
	{
		D3DXVECTOR3 position;
	};

public:
	Model();
	~Model();

	bool Initialize(ID3D11Device*, char*, LPCSTR, LPCSTR, int);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetVertexCount();
	int GetInstanceCount();
	ID3D11ShaderResourceView** GetTextureArray();

	void SetPosition(float, float, float);
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3* GetExtremePositions();

	void SetNewInstanceObject(ID3D11Device*);
private:
	bool InitializeBuffers(ID3D11Device*, int);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, LPCSTR, LPCSTR);
	void ReleaseTexture();

	bool LoadModel(char*);
	void CalcExtremePos(D3DXVECTOR3);

	ID3D11Buffer *m_vertexBuffer, *m_instanceBuffer;
	int m_vertexCount, m_instanceCount;
	ID3D11ShaderResourceView* m_textures[2];
	OBJReader reader;
	D3DXVECTOR3 m_position;
	D3DXVECTOR3* m_extremePositions;
	InstanceType* m_instances;
};

