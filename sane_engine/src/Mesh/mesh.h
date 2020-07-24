#pragma once
#include <glad/glad.h>

struct Mesh
{
	GLuint VAO;
	GLuint verticesVBO;
	GLuint texCoordsVBO;
	GLuint normalsVBO;

	int numVertices;
};

struct MeshIndexed
{
	GLuint VAO;
	GLuint verticesVBO;
	GLuint texCoordsVBO;
	GLuint EBO;

	int numVertices;
	int numIndices;
};


Mesh GenerateMesh(float* vertices, int numVertices, float* texCoords, int numTexCoords, float* normals, int numNormals);
MeshIndexed GenerateMeshIndexed(float* vertices, int numVertices, float* texCoords, int numTexCoords, unsigned int* indices, int numIndices);
void DrawMesh(Mesh& mesh);
void DrawMeshIndexed(MeshIndexed& mesh);

void Clean(Mesh& m);