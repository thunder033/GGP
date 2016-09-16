#include "Material.h"



Material::Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader)
{
	this->vertexShader = vertexShader;
	this->pixelShader = pixelShader;
}


Material::~Material()
{
}
