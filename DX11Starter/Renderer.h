#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include "Material.h"
#include "Entity.h"
#include "Camera.h"
#include <DirectXMath.h>

using namespace DirectX;

class Renderer
{
	ID3D11Device*			device;
	ID3D11DeviceContext*	context;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	ID3D11SamplerState* sampler;

	ID3D11Texture2D* defaultTexture;
	ID3D11ShaderResourceView* defaultSrv;
	Material* baseMaterial;

	void LoadShaders();
	void CreateSampler();
	void CreateDefaultMaterial();
	
public:
	Renderer(ID3D11Device* device, ID3D11DeviceContext* context);
	~Renderer();

	void Render(Entity* entity, Camera* camera);
};

