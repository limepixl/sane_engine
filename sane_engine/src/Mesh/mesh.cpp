#include "mesh.h"

Mesh GenerateMesh(float* vertices, int numVertices, float* texCoords, int numTexCoords, float* normals, int numNormals)
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

	// Generate vertex buffer object for normal data
	GLuint VBO3;
	glGenBuffers(1, &VBO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, numNormals * sizeof(float), normals, GL_STATIC_DRAW);

	// Bind buffer to attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return { VAO, VBO1, VBO2, VBO3, numVertices };
}

void DrawMesh(Mesh& mesh, bool sameModel)
{
	if(!sameModel)
		glBindVertexArray(mesh.VAO);
	glDrawArrays(GL_TRIANGLES, 0, mesh.numVertices);
}

void Clean(Mesh& m)
{
	glDeleteBuffers(1, &m.verticesVBO);
	glDeleteBuffers(1, &m.texCoordsVBO);
	glDeleteBuffers(1, &m.normalsVBO);
	glDeleteVertexArrays(1, &m.VAO);
}
