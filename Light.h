#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "ShadowMap.h"

class Light
{
public:
	Light();
	Light(GLfloat shadowWidth, GLfloat shadowHeight, 
		  GLfloat red, GLfloat green, GLfloat blue, 
		  GLfloat aIntensity, GLfloat dIntensity);

	ShadowMap* GetShadowMap() { return shadowMap; }

	~Light();

protected:
	// Ambient Light
	glm::vec3 color;
	GLfloat ambientIntensity;

	// Diffuse Light
	GLfloat diffuseIntensity;

	// Shadows
	glm::mat4 lightProj;

	ShadowMap* shadowMap;
};

