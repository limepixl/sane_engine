#pragma once
#include <glad/glad.h>

struct Mesh
{
	GLuint VAO;
	GLuint verticesVBO;
	GLuint texCoordsVBO;
	GLuint EBO;

	int numVertices;
	int numIndices;

	~Mesh();
};

Mesh GenerateMesh(float* vertices, int numVertices, float* texCoords, int numTexCoords);
Mesh GenerateMeshIndexed(float* vertices, int numVertices, float* texCoords, int numTexCoords, unsigned int* indices, int numIndices);
void DrawMesh(Mesh* mesh);
void DrawMeshIndexed(Mesh* mesh);