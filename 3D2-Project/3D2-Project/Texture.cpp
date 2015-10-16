#include "Texture.h"


Texture::Texture()
{
	m_texture = 0;
}


Texture::~Texture()
{
}

bool Texture::Initialize(ID3D11Device* device, LPCSTR filename)
{
	HRESULT result;

	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		return false;
	}
	
	return true;
}

void Texture::Shutdown()
{
	m_texture->Release();
	m_texture = 0;
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return m_texture;
}