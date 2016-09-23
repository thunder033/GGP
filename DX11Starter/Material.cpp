#include "Material.h"


Material::Material(SimpleVertexShader * vertexShader, SimplePixelShader * pixelShader, ID3D11ShaderResourceView* srv)
{
	this->vertexShader = vertexShader;
	this->pixelShader = pixelShader;

	this->color = DirectX::XMFLOAT4(1, 1, 1, 1);
	this->textureSrv = srv;
}

Material::Material(SimpleVertexShader * vertexShader, SimplePixelShader * pixelShader, DirectX::XMFLOAT4 color, ID3D11ShaderResourceView* srv)
	: Material(vertexShader, pixelShader, srv)
{
	this->color = color;
}


Material::~Material()
{
}
