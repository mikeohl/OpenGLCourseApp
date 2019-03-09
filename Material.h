#pragma once

#include <GL\glew.h>

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat sPower);

	void UseMaterial(GLuint specularIntensityLocation, GLuint specularPowerLocation);

	~Material();

private:
	GLfloat specularIntensity;
	GLfloat specularPower;
};

