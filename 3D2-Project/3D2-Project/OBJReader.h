#pragma once

#include <Windows.h>
#include <fstream>
#include <vector>
#include <D3DX10math.h>

struct Vertex
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 texture;
	D3DXVECTOR3 normal;
};

struct int3
{
	int x, y, z;
};

class OBJReader
{
public:
	OBJReader();
	~OBJReader();

	bool Load(char* filename);
	std::vector<Vertex> GetVertices();
private:
	void ResetVectors();
	void ReadVertexPos(std::ifstream& file);
	void ReadTexCoord(std::ifstream& file);
	void ReadNormal(std::ifstream& file);
	void ReadFace(std::ifstream& file);
	int3 ConvertFaceValues(std::string);

	std::vector<D3DXVECTOR3> m_positions;
	std::vector<D3DXVECTOR2> m_texCoords;
	std::vector<D3DXVECTOR3> m_normals;
	std::vector<Vertex> m_vertices;
};

