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

Mesh GenerateMesh(float* vertices, int numVertices, float* texCoords, int numTexCoords, float* normals, int numNormals);
Mesh GenerateScreenQuad();
Mesh GenerateCube();
void DrawMesh(Mesh& mesh, bool sameModel);

void Clean(Mesh& m);