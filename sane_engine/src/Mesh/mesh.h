#pragma once
#include <glad/glad.h>

struct Mesh
{
	GLuint VAO;
	GLuint verticesVBO;
	GLuint EBO;

	int numVertices;
	int numIndices;

	~Mesh();
};

Mesh GenerateMesh(float* vertices, int numVertices, unsigned int* indices, int numIndices);
void DrawMesh(Mesh* mesh);