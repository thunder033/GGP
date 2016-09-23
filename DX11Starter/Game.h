#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Mesh.h"
#include "Entity.h"
#include "Camera.h"
#include "Lights.h"
#include "Renderer.h"

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void CreateBasicGeometry();

	std::vector<Entity*> entities;
	Renderer* renderer;

	//Meshes
	const int meshCount = 9;
	Mesh** meshes;

	//Textures
	ID3D11ShaderResourceView* defaultSrv;
	ID3D11ShaderResourceView* crateSrv;
	ID3D11SamplerState* sampler;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	//Rendering Data
	ID3D11Texture2D* defaultTexture;
	Material* baseMaterial;
	Material* crate;
	Material* blue;
	Camera* camera;
	DirectionalLight* lights;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
	bool mouseDragging;
};

