#include <DirectXMath.h>

#pragma once
class Transform
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 rotation;

	DirectX::XMFLOAT4X4 matrix;
	bool dirty;

public:
	Transform();
	~Transform();

	void SetPosition(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	void SetPosition(DirectX::XMFLOAT3 position);
	void SetScale(DirectX::XMFLOAT3 scale);
	void SetRotation(DirectX::XMFLOAT3 rotation);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetScale();
	DirectX::XMFLOAT3 GetRotation();

	DirectX::XMFLOAT4X4 GetMatrix();
};

