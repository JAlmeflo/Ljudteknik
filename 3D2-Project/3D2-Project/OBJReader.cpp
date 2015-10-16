#include "OBJReader.h"


OBJReader::OBJReader()
{
}


OBJReader::~OBJReader()
{
}

bool OBJReader::Load(char* filename)
{
	ResetVectors();

	std::ifstream file;
	file.open(filename);

	if (!file)
	{
		MessageBox(0, "Could not find OBJ file", filename, MB_ICONERROR | MB_OK);
		return false;
	}


	char* charp = new char();
	std::string str;
	while (!file.eof())
	{
		file >> charp;
		str = charp;
		if (str == "v")
			ReadVertexPos(file);
		else if (str == "vt")
			ReadTexCoord(file);
		else if (str == "vn")
			ReadNormal(file);
		else if (str == "f")
			ReadFace(file);
	}

	return true;
}

void OBJReader::ResetVectors()
{
	m_positions = std::vector<D3DXVECTOR3>();
	m_texCoords = std::vector<D3DXVECTOR2>();
	m_normals = std::vector<D3DXVECTOR3>();
	m_vertices = std::vector<Vertex>();
}

void OBJReader::ReadVertexPos(std::ifstream& file)
{
	D3DXVECTOR3 vertex;

	file >> vertex.x;
	file >> vertex.y;
	file >> vertex.z;


	m_positions.push_back(vertex);
}

void OBJReader::ReadTexCoord(std::ifstream& file)
{
	D3DXVECTOR2 texCoord;

	file >> texCoord.x;
	file >> texCoord.y;

	m_texCoords.push_back(texCoord);
}

void OBJReader::ReadNormal(std::ifstream& file)
{
	D3DXVECTOR3 normal;

	file >> normal.x;
	file >> normal.y;
	file >> normal.z;

	m_normals.push_back(normal);
}

void OBJReader::ReadFace(std::ifstream& file)
{
    std::string str;
    char* cp = new char();
	Vertex point;

    for (int i = 0; i < 3; i++)
    {
        file >> cp;
        str = cp;
        int3 indicies = ConvertFaceValues(str);

		point.position = m_positions[indicies.x - 1];
        point.texture = m_texCoords[indicies.y - 1];
        point.normal = m_normals[indicies.z - 1];

		m_vertices.push_back(point);
    }
}

int3 OBJReader::ConvertFaceValues(std::string str)
{
	int3 values;
	std::string string1 = "", string2 = "", string3 = "";
	int n = 0;
	for (unsigned int i = 0; i < str.size(); i++)
	{
		char token = str[i];
		if (token == '/')
		{
			n++;
		}
		else
		{
			switch (n)
			{
			case 0:
				string1 += token;
				break;
			case 1:
				string2 += token;
				break;
			case 2:
				string3 += token;
				break;
			}
		}
	}

    values.x = atoi(string1.c_str());
    values.y = atoi(string2.c_str());
    values.z = atoi(string3.c_str());

    return values;
}

std::vector<Vertex> OBJReader::GetVertices()
{
	return m_vertices;
}