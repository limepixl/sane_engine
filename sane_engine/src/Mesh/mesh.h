#pragma once
#include <glad/glad.h>

struct Mesh
{
	GLuint VAO;
	GLuint verticesVBO;
	GLuint texCoordsVBO;

	int numVertices;

	~Mesh();
};

struct MeshIndexed
{
	GLuint VAO;
	GLuint verticesVBO;
	GLuint texCoordsVBO;
	GLuint EBO;

	int numVertices;
	int numIndices;

	~MeshIndexed();
};


Mesh GenerateMesh(float* vertices, int numVertices, float* texCoords, int numTexCoords);
MeshIndexed GenerateMeshIndexed(float* vertices, int numVertices, float* texCoords, int numTexCoords, unsigned int* indices, int numIndices);
void DrawMesh(Mesh& mesh);
void DrawMeshIndexed(MeshIndexed& mesh);