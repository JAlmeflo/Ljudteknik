#pragma once

// Globals
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 500.0f;
const float SCREEN_NEAR = 1.0f;

const int SHADOWMAP_WIDTH = 2048;
const int SHADOWMAP_HEIGHT = 2048;

#include <Windows.h>
#include <string>
#include "D3DClass.h"
#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "Light.h"
#include "RenderTexture.h"
#include "DepthShader.h"
#include "Frustum.h"

class Graphics
{
public:
	Graphics();
	~Graphics();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
	void Update(float);
private :
	bool RenderSceneToTexture();
	bool Render(float);
	void SetWindowTitle();

	HWND m_hwnd;
	D3DClass* m_D3D;
	Camera* m_camera;
	//Model* m_ground;
	std::vector<Model*> m_models;
	std::vector<Model*> m_billboadModels;
	RenderTexture* m_renderTexture;
	DepthShader* m_depthShader;
	Shader* m_shadowShader;
	Light* m_light;
	float rotation;
	Frustum* m_frustum;
	int m_modelCount;
};

