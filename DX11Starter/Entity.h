#pragma once

#include "Mesh.h"
#include "Transform.h"

class Entity
{
	Mesh* mesh;
	Transform* transform;

public:
	Entity(Mesh* mesh);
	virtual ~Entity();

	Mesh* GetMesh();
	Transform* GetTransform();

	DirectX::XMFLOAT4X4 GetDrawMatrix();
};

