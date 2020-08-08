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

struct MeshIndexed
{
	uint32_t VAO;
	uint32_t verticesVBO;
	uint32_t EBO;
	uint32_t texCoordsVBO;
	uint32_t normalsVBO;

	int numIndices;
};

Mesh GenerateMesh(float* vertices, int numVertices, float* texCoords, int numTexCoords, float* normals, int numNormals);
MeshIndexed GenerateMeshIndexed(float* vertices, int numVertices, unsigned int* indices, int numIndices, float* texCoords, int numTexCoords, float* normals, int numNormals);
Mesh GenerateScreenQuad();
Mesh GenerateCube();
void DrawMesh(Mesh& mesh, bool sameModel);
void DrawMeshIndexed(MeshIndexed& mesh, bool sameModel);

void CleanMesh(Mesh& m);
void CleanMeshIndexed(MeshIndexed& m);