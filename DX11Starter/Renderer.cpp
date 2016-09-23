#include "Renderer.h"



void Renderer::CreateDefaultMaterial()
{
	unsigned char textureColor[] = { 255, 255, 255, 255 };

	//Create the default texture description
	D3D11_TEXTURE2D_DESC defaultTextureDesc;
	ZeroMemory(&defaultTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	defaultTextureDesc.Width = 1;
	defaultTextureDesc.Height = 1;
	defaultTextureDesc.MipLevels = 1;
	defaultTextureDesc.ArraySize = 1;
	defaultTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	defaultTextureDesc.SampleDesc.Count = 1;
	defaultTextureDesc.SampleDesc.Quality = 0;
	defaultTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	defaultTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	defaultTextureDesc.CPUAccessFlags = 0;
	defaultTextureDesc.MiscFlags = 0;

	//Set the initial data of a white color
	D3D11_SUBRESOURCE_DATA defaultTextureInitData;
	ZeroMemory(&defaultTextureInitData, sizeof(D3D11_SUBRESOURCE_DATA));
	defaultTextureInitData.pSysMem = textureColor;
	defaultTextureInitData.SysMemPitch = sizeof(unsigned char) * 4;

	//Create the Texture and SRV
	defaultTexture = nullptr;
	device->CreateTexture2D(&defaultTextureDesc, &defaultTextureInitData, &defaultTexture);
	device->CreateShaderResourceView(defaultTexture, NULL, &defaultSrv);

	baseMaterial = new Material(vertexShader, pixelShader, defaultSrv);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Renderer::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	if (!vertexShader->LoadShaderFile(L"Debug/VertexShader.cso"))
		vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	if (!pixelShader->LoadShaderFile(L"Debug/PixelShader.cso"))
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

void Renderer::CreateSampler()
{
	//Create a sampler state for texture sampling
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &sampler);
}

Renderer::Renderer(ID3D11Device* device, ID3D11DeviceContext* context)
{
	this->device = device;
	this->context = context;

	LoadShaders();
	CreateSampler();
	CreateDefaultMaterial();
}



Renderer::~Renderer()
{
}
