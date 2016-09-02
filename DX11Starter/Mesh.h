#include "DXCore.h"
#include "Vertex.h"

#pragma once
class Mesh
{
private:
	// Buffers to hold actual geometry data
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int indexCount;

public:

	Mesh(int indices[], Vertex vertices[], int indexCount, int vertexCount, ID3D11Device* device);
	~Mesh();

	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexCount();
};

