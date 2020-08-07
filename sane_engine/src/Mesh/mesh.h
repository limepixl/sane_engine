#pragma once
#include <inttypes.h>

struct Mesh
{
	uint32_t VAO;
	uint32_t verticesVBO;
	uint32_t texCoordsVBO;
	uint32_t normalsVBO;

	int numVertices;
};

Mesh GenerateMesh(float* vertices, int numVertices, float* texCoords, int numTexCoords, float* normals, int numNormals);
Mesh GenerateScreenQuad();
Mesh GenerateCube();
void DrawMesh(Mesh& mesh, bool sameModel);

void Clean(Mesh& m);