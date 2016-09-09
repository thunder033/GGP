#include "Entity.h"



Entity::Entity(Mesh* mesh)
{
	this->mesh = mesh;
	transform = new Transform();
}


Entity::~Entity()
{
	if (transform != nullptr) {
		delete transform;
		transform = nullptr;
	}
}

Mesh * Entity::GetMesh()
{
	return mesh;
}

Transform * Entity::GetTransform()
{
	return transform;
}

DirectX::XMFLOAT4X4 Entity::GetDrawMatrix()
{
	//DirectX::XMMATRIX W = DirectX::XMMATRIX(transform->GetMatrix());
	DirectX::XMMATRIX W = DirectX::XMLoadFloat4x4(&transform->GetMatrix());
	DirectX::XMFLOAT4X4 matrix;
	DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixTranspose(W)); // Transpose for HLSL!
	return matrix;
}