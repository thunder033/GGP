#pragma once

#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include <DirectXMath.h>

class Entity
{
	Mesh* mesh;
	Material* material;
	Transform* transform;

public:
	Entity(Mesh* mesh, Material* material);
	virtual ~Entity();

	Mesh* GetMesh();
	Transform* GetTransform();
	void PrepareMaterial(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix, ID3D11ShaderResourceView* textureSrv, ID3D11SamplerState* sampler);

	DirectX::XMFLOAT4X4 GetDrawMatrix();
};

