#include "Transform.h"



Transform::Transform()
{
	position = DirectX::XMFLOAT3(0, 0, 0);
	scale = DirectX::XMFLOAT3(1, 1, 1);
	rotation = DirectX::XMFLOAT3(0, 0, 0);
	dirty = true;
}


Transform::~Transform()
{
}


DirectX::XMFLOAT3 Transform::GetPosition()
{
	return position;
}

DirectX::XMFLOAT3 Transform::GetScale()
{
	return scale;
}

DirectX::XMFLOAT3 Transform::GetRotation()
{
	return rotation;
}

void Transform::SetPosition(DirectX::XMFLOAT3 position)
{
	this->position = position;
	dirty = true;
}

void Transform::SetRotation(DirectX::XMFLOAT3 rotation)
{
	this->rotation = rotation;
	dirty = true;
}

void Transform::SetScale(DirectX::XMFLOAT3 scale)
{
	this->scale = scale;
	dirty = true;
}

void Transform::SetPosition(float x, float y, float z)
{
	SetPosition(DirectX::XMFLOAT3(x, y, z));
}

void Transform::SetScale(float x, float y, float z)
{
	SetScale(DirectX::XMFLOAT3(x, y, z));
}

void Transform::SetRotation(float x, float y, float z)
{
	SetRotation(DirectX::XMFLOAT3(x, y, z));
}

DirectX::XMFLOAT4X4 Transform::GetMatrix()
{
	if (!dirty) {
		return matrix;
	}
	else {
		dirty = false;
		DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixTransformation(
			DirectX::XMVECTOR(),			//Scaling Origin
			DirectX::XMVECTOR(),			//Scaling Orientation Quaterion
			DirectX::XMLoadFloat3(&scale),  //Scaling
			DirectX::XMVECTOR(),			//Rotation Origin
			DirectX::XMLoadFloat3(&rotation),//Rotation Quaternion
			DirectX::XMLoadFloat3(&position) //Translation
		));
		return matrix;
	}
}
