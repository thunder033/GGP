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
	Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader);
	virtual ~Material();

	SimpleVertexShader* GetVertexShader() {
		return vertexShader;
	};

	SimplePixelShader* GetPixelShader() {
		return pixelShader;
	}
};

