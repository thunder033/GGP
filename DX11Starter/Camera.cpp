#include "Camera.h"
#include <iostream>

// For the DirectX Math library
using namespace DirectX;

Camera::Camera(float aspectRatio)
{
	transform = new Transform();
	transform->SetPosition(0, 0, -8);
	transform->SetRotation(0, 0, 1);
	this->aspectRatio = aspectRatio;
	UpdateViewMatrix();
}

void Camera::SetAspectRatio(float aspectRatio) 
{
	this->aspectRatio = aspectRatio;

	// Create/Update the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//   the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		aspectRatio,			// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

Camera::~Camera()
{
	if (transform != nullptr) {
		delete transform;
		transform = nullptr;
	}
}

void Camera::Update(double deltaTime, double elapsedTime)
{
	float speed = 15.0f;
	if (GetAsyncKeyState('W') & 0x8000) {
		this->Advance((float)deltaTime * speed);
	}

	if (GetAsyncKeyState('S') & 0x8000) {
		this->Advance((float)-deltaTime * speed);
	}

	if (GetAsyncKeyState('A') & 0x8000) {
		this->Strafe((float)deltaTime * speed);
	}

	if (GetAsyncKeyState('D') & 0x8000) {
		this->Strafe((float)-deltaTime * speed);
	}

	if (GetAsyncKeyState('E') & 0x8000) {
		this->Ascend((float)deltaTime * speed);
	}

	if (GetAsyncKeyState('C') & 0x8000) {
		this->Ascend((float)-deltaTime * speed);
	}

	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMLoadFloat3(&transform->GetPosition());

	XMFLOAT3 rotation = transform->GetRotation();
	XMVECTOR dir = XMQuaternionRotationRollPitchYaw( rotation.x, rotation.y, 3.14f );
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!
}

//Move the camera forward(+)/back(-) the by the given amount
void Camera::Advance(float units)
{
	XMVECTOR position = XMLoadFloat3(&transform->GetPosition());
	XMVECTOR forward = XMQuaternionMultiply(XMVectorSet(0, 0, 1, 0), transform->GetOrientation());
	position += forward * units;
	transform->SetPosition(position);
}

//Move the camera right(+)/left(-) by the given amount
void Camera::Strafe(float units)
{
	XMVECTOR position = XMLoadFloat3(&transform->GetPosition());
	XMVECTOR forward = XMQuaternionMultiply(XMVectorSet(0, 0, 1, 0), transform->GetOrientation());
	XMVECTOR right = XMVector3Cross(forward, XMVectorSet(0, 1, 0, 0));
	position += right * units;
	transform->SetPosition(position);
}

//Move the camera up(+) or down(-) the world Y axis by the given amount
void Camera::Ascend(float units)
{
	XMVECTOR position = XMLoadFloat3(&transform->GetPosition());
	position += XMVectorSet(0, 1, 0, 0) * units;
	transform->SetPosition(position);
}

void Camera::Rotate(float x, float y)
{
	XMFLOAT3 rotation = transform->GetRotation();
	float FOUR_PI = 12.56f; //I'm not sure why a full rotation is 4*PI instead of 2*PI...
	transform->SetRotation(fmod(rotation.x + x, FOUR_PI), fmod(rotation.y + y, FOUR_PI), 0);
}