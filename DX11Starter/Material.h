#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>

class Material
{
	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
public:
	Material();
	~Material();
};

