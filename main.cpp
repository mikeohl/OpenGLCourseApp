#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION

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

#include <assimp\Importer.hpp>

#include "CommonValues.h"

#include "GLWindow.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"

const float toRadians = M_PI / 180.0f;

GLWindow mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

Material shinyMaterial;
Material dullMaterial;

Model xwing;
Model blackhawk;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void calcAverageNormals(unsigned int * indices, unsigned int indiceCount,
	                    GLfloat * vertices, unsigned int verticeCount,
	                    unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;

		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in2 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
		
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

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
	//    x     y     z        u     v       nx    ny    nz
		-1.0f, -1.0f, -0.5f,  0.0f, 0.0f,   0.0f, 0.0f, 0.0f, // 0, left
		0.0f, -1.0f, 1.0f,	  0.5f, 0.0f,   0.0f, 0.0f, 0.0f, // 1, back
		1.0f, -1.0f, -0.5f,	  1.0f, 0.0f,   0.0f, 0.0f, 0.0f, // 2, right
		0.0f, 1.0f, 0.0f,     0.5f, 1.0f,   0.0f, 0.0f, 0.0f, // 3, top
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,  10.0f, 0.0f,  0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,  0.0f, 10.0f,  0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,   10.0f, 10.0f, 0.0f, -1.0f, 0.0f
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12); // TODO: Pass in numbers through model files
	meshList.push_back(obj1); // Add to back of global mesh list

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = GLWindow(1366, 768);
	mainWindow.initialize();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.2f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();

	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 4);

	xwing = Model();
	xwing.LoadModel("Models/x-wing.obj");

	blackhawk = Model();
	blackhawk.LoadModel("Models/uh60.obj");

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 
		                         0.2, 0.3f,
		                         2.0f, -1.0f, -2.0f);

	unsigned int pointLightCount = 0;

	pointLights[0] = PointLight(0.0f, 0.0f, 1.0f,
		                        0.1f, 0.1f,
		                       -4.0f, 2.0f, 0.0f,
		                        0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
		                        0.1f, 0.1f,
		                        4.0f, 2.0f, 0.0f,
		                        0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;

	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		                      0.0f, 2.0f,
		                      0.0f, 0.0f, 0.0f,
		                      0.0f,-1.0f, 0.0f,
		                      1.0f, 0.0f, 0.0f,
		                      20.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		                      0.0f, 1.0f,
		                      2.0f, 0.0f, 0.0f,
	                       -10.0f, -1.0f, 0.0f,
		                      1.0f, 0.0f, 0.0f,
		                     20.0f);
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		   uniformSpecularIntensity = 0, uniformSpecularPower = 0;

	glm::mat4 projection = glm::perspective<float>(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		// Get and Handle user input events
		glfwPollEvents();

		// Set Camera Keys
		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformSpecularPower = shaderList[0].GetSpecularPowerLocation();

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		
		//printf("%f \n", model[0][0]);

		//glUniform1f(uniformModel, triOffset);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		brickTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformSpecularPower);
		meshList[0]->RenderMesh();

		// Render bottom pyramid
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		dirtTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformSpecularPower);
		meshList[1]->RenderMesh();

		// Render top pyramid
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		dirtTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformSpecularPower);
		meshList[2]->RenderMesh();

		// render xwing model
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-6.0f, 0.0f, 10.0f));
		model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformSpecularPower);
		xwing.RenderModel();

		// render blackhawk model
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformSpecularPower);
		blackhawk.RenderModel();

		glUseProgram(0); //Undersigning the shader

		mainWindow.swapBuffers();
	}

	return 0;
}


// TODO: Incomplete function
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