#include "PointLight.h"



PointLight::PointLight()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear   = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue,
	                   GLfloat aIntensity, GLfloat dIntensity,
	                   GLfloat xPos, GLfloat yPos, GLfloat zPos,
	                   GLfloat con, GLfloat lin, GLfloat exp) : Light(red, green, blue, aIntensity, dIntensity)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = con;
	linear = lin;
	exponent = exp;
}

void PointLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation,
	          GLfloat diffuseIntensityLocation, GLfloat positionLocation,
	          GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation)
{
	// Ambient
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	// Diffuse
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	// Position
	glUniform3f(positionLocation, position.x, position.y, position.z);

	// Intensity factors
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}

PointLight::~PointLight()
{
}
