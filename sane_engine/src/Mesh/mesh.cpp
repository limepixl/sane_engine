#include "mesh.h"

Mesh::~Mesh()
{
	glDeleteBuffers(1, &verticesVBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

Mesh GenerateMesh(float* vertices, int numVertices, float* texCoords, int numTexCoords)
{
	// Generate vertex array object
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate vertex buffer object for position data
	GLuint VBO1;
	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float), vertices, GL_STATIC_DRAW);

	// Bind buffer to attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Generate vertex buffer object for texture coordinate data
	GLuint VBO2;
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, numTexCoords * sizeof(float), texCoords, GL_STATIC_DRAW);

	// Bind buffer to attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return { VAO, VBO1, VBO2, 0, numVertices, 0 };
}

Mesh GenerateMeshIndexed(float* vertices, int numVertices, float* texCoords, int numTexCoords, unsigned int* indices, int numIndices)
{
	// Generate vertex array object
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate vertex buffer object for position data
	GLuint VBO1;
	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float), vertices, GL_STATIC_DRAW);

	// Bind buffer to attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Generate vertex buffer object for texture coordinate data
	GLuint VBO2;
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, numTexCoords * sizeof(float), texCoords, GL_STATIC_DRAW);

	// Bind buffer to attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	// Generate element buffer object for indices
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return {VAO, VBO1, VBO2, EBO, numVertices, numIndices};
}

void DrawMesh(Mesh* mesh)
{
	glBindVertexArray(mesh->VAO);
	glDrawArrays(GL_TRIANGLES, 0, mesh->numVertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DrawMeshIndexed(Mesh* mesh)
{
	glBindVertexArray(mesh->VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
	glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, nullptr);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


