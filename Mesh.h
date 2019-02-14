#pragma once

#include <GL\glew.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, 
					unsigned int *indices, 
					unsigned int numOfVeritices, 
					unsigned int numOfIndices
		);
	void RenderMesh();
	void ClearMesh();

	~Mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};

// Acronym Def
// VA0 (Vertex Array Object)
// VBO (Vertex Buffer Object)
// IBO (Index Buffer Object)
