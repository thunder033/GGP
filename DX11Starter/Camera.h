#pragma once
#include "DXCore.h"
#include "Transform.h"
#include <DirectXMath.h>

class Camera
{
	float aspectRatio;
	Transform* transform;

	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;
public:
	Camera(float aspectRatio);
	~Camera();

	void Update(double deltaTime, double elapsedTime);

	void SetAspectRatio(float aspectRatio);

	void UpdateViewMatrix();

	void Advance(float units);

	void Strafe(float units);

	void Ascend(float units);

	void Rotate(float x, float y);

	DirectX::XMFLOAT4X4 getViewMatrix() {
		return viewMatrix;
	}

	DirectX::XMFLOAT4X4 getProjectionMatrix() {
		return projectionMatrix;
	}
};

