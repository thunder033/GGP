#include "Entity.h"



Entity::Entity(Mesh* mesh, Material* material)
{
	this->mesh = mesh;
	this->material = material;
	transform = new Transform();
}


Entity::~Entity()
{
	if (transform != nullptr) {
		delete transform;
		transform = nullptr;
	}
}

Mesh * Entity::GetMesh()
{
	return mesh;
}

Transform * Entity::GetTransform()
{
	return transform;
}

DirectX::XMFLOAT4X4 Entity::GetDrawMatrix()
{
	DirectX::XMMATRIX W = DirectX::XMLoadFloat4x4(&transform->GetMatrix());
	DirectX::XMFLOAT4X4 matrix;
	DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixTranspose(W)); // Transpose for HLSL!
	return matrix;
}

void Entity::PrepareMaterial(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix, ID3D11ShaderResourceView* textureSrv, ID3D11SamplerState* sampler)
{
	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.
	material->GetVertexShader()->SetMatrix4x4("world", GetDrawMatrix());
	material->GetVertexShader()->SetMatrix4x4("view", viewMatrix);
	material->GetVertexShader()->SetMatrix4x4("projection", projectionMatrix);


	material->GetPixelShader()->SetSamplerState("Sampler", sampler);
	material->GetPixelShader()->SetShaderResourceView("Texture", textureSrv);

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	material->GetVertexShader()->CopyAllBufferData();
	material->GetPixelShader()->CopyAllBufferData();

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	material->GetVertexShader()->SetShader();
	material->GetPixelShader()->SetShader();

}