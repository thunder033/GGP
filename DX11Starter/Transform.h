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

	void Translate(float x, float y, float z);
	void Rotate(float x, float y, float z);
	void Scale(float x, float y, float z);

	/*void Translate(DirectX::XMVECTOR translation);
	void Rotate(DirectX::XMVECTOR rotation);
	void Scale(DirectX::XMVECTOR scale);*/

	void SetPosition(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	void SetPosition(DirectX::XMFLOAT3 position);
	void SetScale(DirectX::XMFLOAT3 scale);
	void SetRotation(DirectX::XMFLOAT3 rotation);

	void SetPosition(DirectX::XMVECTOR position);
	void SetScale(DirectX::XMVECTOR scale);
	void SetRotation(DirectX::XMVECTOR rotation);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetScale();
	DirectX::XMFLOAT3 GetRotation();

	DirectX::XMVECTOR GetOrientation();

	DirectX::XMFLOAT4X4 GetMatrix();
};

