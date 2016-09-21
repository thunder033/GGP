#include "Game.h"
#include "Vertex.h"
#include "WICTextureLoader.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore( 
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields
	vertexShader = 0;
	pixelShader = 0;
	mouseDragging = false;
	camera = new Camera((float)width / height);

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{

	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;
	delete baseMaterial;
	delete camera;

	sampler->Release();
	textureSrv->Release();

	std::vector<Entity*>::iterator it;
	for (it = entities.begin(); it < entities.end(); it++) {
		delete *it;
	}

	if (meshes != nullptr) {
		for (int i = 0; i < meshCount; i++) {
			if (meshes[i] != nullptr) {
				delete meshes[i];
			}
		}
		delete meshes;
	}
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	camera->SetAspectRatio((float)width / height);
	CreateBasicGeometry();

	//Texturing
	CreateWICTextureFromFile(
		device, 
		context, //Providing the context will auto-generate mipmaps
		L"Debug/Textures/crate.png", 
		0, //we don't actually need the texture reference
		&textureSrv);

	//Create a sampler state for texture sampling
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &sampler);

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	if (!vertexShader->LoadShaderFile(L"Debug/VertexShader.cso"))
		vertexShader->LoadShaderFile(L"VertexShader.cso");		

	pixelShader = new SimplePixelShader(device, context);
	if(!pixelShader->LoadShaderFile(L"Debug/PixelShader.cso"))	
		pixelShader->LoadShaderFile(L"PixelShader.cso");

	// You'll notice that the code above attempts to load each
	// compiled shader file (.cso) from two different relative paths.

	// This is because the "working directory" (where relative paths begin)
	// will be different during the following two scenarios:
	//  - Debugging in VS: The "Project Directory" (where your .cpp files are) 
	//  - Run .exe directly: The "Output Directory" (where the .exe & .cso files are)

	// Checking both paths is the easiest way to ensure both 
	// scenarios work correctly, although others exist
}

// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red	= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green	= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue	= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 yellow = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	Vertex triangleVerts[] = 
	{
		{ XMFLOAT3(+0.0f, +1.0f, +0.0f), red, XMFLOAT2(0, 0) },
		{ XMFLOAT3(+1.5f, -1.0f, +0.0f), blue, XMFLOAT2(1, 1) },
		{ XMFLOAT3(-1.5f, -1.0f, +0.0f), green, XMFLOAT2(0, 1) },
	};

	Vertex cubeVerts[] =
	{
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), yellow, XMFLOAT2(1, 0) }, //FTR - 0
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f), blue, XMFLOAT2(1, 1) }, //FBR - 1
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), green, XMFLOAT2(0, 1) }, //FBL - 2
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), red, XMFLOAT2(0, 0) }, //FTL - 3

		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), yellow, XMFLOAT2(0, 0) }, //BTR - 4
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), blue, XMFLOAT2(0, 1) }, //BBR - 5
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), green, XMFLOAT2(1, 1) }, //BBL - 6
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), red, XMFLOAT2(1, 0) } //BTL - 7
	};

	Vertex hexagonVerts[] =
	{
		{ XMFLOAT3(+ 0.0f, + 0.0f, +0.0f), red, XMFLOAT2(0, 0) }, //Center

		{ XMFLOAT3(- 0.4f, + 0.6f, +0.0f), blue, XMFLOAT2(1, 0) }, //TL
		{ XMFLOAT3(+ 0.4f, + 0.6f, +0.0f), blue, XMFLOAT2(1, 0) }, //TR
		{ XMFLOAT3(+ 0.75f, + 0.0f, +0.0f), blue, XMFLOAT2(1, 1) }, //MR
		{ XMFLOAT3(+ 0.4f, - 0.6f, +0.0f), blue, XMFLOAT2(0, 1) }, //BR
		{ XMFLOAT3(- 0.4f, - 0.6f, +0.0f), blue, XMFLOAT2(0, 1) }, //BL
		{ XMFLOAT3(- 0.75f, + 0.0f, +0.0f), blue, XMFLOAT2(1, 1) }, //ML
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	int triangleIndices[] = { 0, 1, 2 };
	int cubeIndices[] = { 
		0, 1, 2, 2, 3, 0, //Front
		7, 6, 5, 5, 4, 7, //Back
		5, 1, 0, 4, 5, 0, //Right
		7, 3, 2, 6, 7, 2, //Left
		4, 0, 3, 7, 4, 3, //Top
		2, 1, 5, 2, 5, 6, //Bottom
	};
	int hexagonIndices[] = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 1};

	meshes = new Mesh*[3]{
		new Mesh(triangleIndices, triangleVerts, 3, 3, device),
		new Mesh(cubeIndices, cubeVerts, 36, 8, device),
		new Mesh(hexagonIndices, hexagonVerts, 18, 7, device)
	};

	Material* baseMaterial = new Material(vertexShader, pixelShader);
	entities.push_back(new Entity(meshes[0], baseMaterial));
	entities.push_back(new Entity(meshes[1], baseMaterial)); //cube
	entities.push_back(new Entity(meshes[2], baseMaterial));
	entities.push_back(new Entity(meshes[2], baseMaterial));
	entities.push_back(new Entity(meshes[2], baseMaterial));

	entities[0]->GetTransform()->SetPosition(1.5f, 0, 0);
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Update our projection matrix since the window size changed
	camera->SetAspectRatio((float)width / height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	camera->Update(deltaTime, totalTime);

	entities[0]->GetTransform()->SetPosition(sin(totalTime), 0, 0);
	entities[0]->GetTransform()->SetScale(abs(sin(totalTime)), 1, 1);

	entities[1]->GetTransform()->SetPosition(0, 0, 0);
	entities[1]->GetTransform()->SetPosition(0, sin(totalTime), 0);
	entities[1]->GetTransform()->SetRotation(sin(totalTime) * 3.14f, sin(totalTime) * 3.14f, sin(totalTime) * 3.14f);
	entities[2]->GetTransform()->SetPosition(0, 0, sin(totalTime));

	entities[3]->GetTransform()->SetPosition(cos(totalTime), sin(totalTime), 0);
	entities[4]->GetTransform()->SetPosition(0, cos(totalTime), sin(totalTime));
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	std::vector<Entity*>::iterator it;
	for (it = entities.begin(); it < entities.end(); it++) {

		(*it)->PrepareMaterial(camera->getViewMatrix(), camera->getProjectionMatrix(), textureSrv, sampler);

		// Set buffers in the input assembler
		//  - Do this ONCE PER OBJECT you're drawing, since each object might
		//    have different geometry.
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		Mesh* mesh = (*it)->GetMesh();
		ID3D11Buffer* vertexBuffer = mesh->GetVertexBuffer();
		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		context->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

		// Finally do the actual drawing
		//  - Do this ONCE PER OBJECT you intend to draw
		//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
		//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
		//     vertices in the currently set VERTEX BUFFER
		context->DrawIndexed(
			mesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices

	}

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	if (buttonState & 0x0001) {
		mouseDragging = true;
	}

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	if (!buttonState & 0x0001) {
		mouseDragging = false;
	}

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	if (mouseDragging) {
		float rotateSpeed = 0.005f;
		camera->Rotate((y - prevMousePos.y) * rotateSpeed, (x - prevMousePos.x) * rotateSpeed);
	}

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion