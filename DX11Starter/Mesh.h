#include "DXCore.h"
#include "Vertex.h"
#include <fstream>
#include <vector>

#pragma once
class Mesh
{
private:
	// Buffers to hold actual geometry data
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int indexCount;

public:

	Mesh(UINT indices[], Vertex vertices[], int indexCount, int vertexCount, ID3D11Device* device);
	Mesh(char* modelName, ID3D11Device* device);
	~Mesh();

	void InitBuffers(UINT* indices, Vertex* verticies, int indexCount, int vertexCount, ID3D11Device* device);
	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexCount();
};

