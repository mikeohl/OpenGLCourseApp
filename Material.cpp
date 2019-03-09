#include "Material.h"



Material::Material()
{
	specularIntensity = 0.0f;
	specularPower = 0.0f;
}

Material::Material(GLfloat sIntensity, GLfloat sPower)
{
	specularIntensity = sIntensity;
	specularPower = sPower;
}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint specularPowerLocation)
{
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(specularPowerLocation, specularPower);
}


Material::~Material()
{
}
