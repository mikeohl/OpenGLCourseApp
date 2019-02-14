#define _USE_MATH_DEFINES

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
// #include <glm/mat4x4.hpp>

#include "GLWindow.h"
#include "Mesh.h"
#include "Shader.h"

const float toRadians = M_PI / 180.0f;

GLWindow mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateObjects()
{
	// triangles drawn counter-clockwise for outside face
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	// 4 pyramid vertices
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f, // 0, left
		0.0f, -1.0f, 1.0f,	// 1, back
		1.0f, -1.0f, 0.0f,	// 2, right
		0.0f, 1.0f, 0.0f	// 3, top
	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12); // TODO: Pass in numbers through model files
	meshList.push_back(obj1); // Add to back of global mesh list

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj2);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = GLWindow(800, 600);
	mainWindow.initialize();

	CreateObjects();
	CreateShaders();

	GLuint uniformProjection = 0, uniformModel = 0;
	glm::mat4 projection = glm::perspective<float>(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		// Get and Handle user input events
		glfwPollEvents();

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		
		//printf("%f \n", model[0][0]);

		//glUniform1f(uniformModel, triOffset);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		glUseProgram(0); //Undersigning the shader

		mainWindow.swapBuffers();
	}

	return 0;
}

void animate() 
{
	// translation movement
	bool direction = true;
	float triOffset = 0.0f;
	float triMaxOffset = 0.7f;
	float triIncrement = 0.0005f;

	// rotation movement
	float curAngle = 0.0f;

	// scale variation
	bool sizeDirection = true;
	float curSize = 0.4f;
	float maxSize = 0.8f;
	float minSize = 0.1f;

	// Move the triangle back and forth across the screen
	if (direction)
	{
		triOffset += triIncrement;
	}
	else
	{
		triOffset -= triIncrement;
	}

	if (abs(triOffset) >= triMaxOffset)
	{
		direction = !direction;
	}

	curAngle += 0.01f;
	if (curAngle >= 360)
	{
		curAngle -= 360; // prevent overflow
	}

	if (sizeDirection)
	{
		curSize += 0.0001f;
	}
	else
	{
		curSize -= 0.0001f;
	}

	if (curSize >= maxSize || curSize <= minSize)
	{
		sizeDirection = !sizeDirection;
	}
}