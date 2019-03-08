#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
	      GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, // Using ambientColor as diffuseColor as well
	              GLfloat diffuseIntensityLocation, GLfloat directionLocation);

	~Light();

private:
	// Ambient Light
	glm::vec3 color;
	GLfloat ambientIntensity;

	// Diffuse Light
	glm::vec3 direction;
	GLfloat diffuseIntensity;
};

