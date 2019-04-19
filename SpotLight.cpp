#include "SpotLight.h"



SpotLight::SpotLight() : PointLight()
{
	direction = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
	edge = 0.0f;
	processedEdge = cosf(glm::radians(edge));
	isOn = true;
}

SpotLight::SpotLight(GLuint shadowWidth, GLuint shadowHeight,
	                 GLfloat near, GLfloat far, GLfloat red, GLfloat green, GLfloat blue,
	                 GLfloat aIntensity, GLfloat dIntensity,
	                 GLfloat xPos, GLfloat yPos, GLfloat zPos,
	                 GLfloat xDir, GLfloat yDir, GLfloat zDir,
	                 GLfloat con, GLfloat lin, GLfloat exp,
                     GLfloat edge) : PointLight(shadowWidth, shadowHeight, near, far, red, green, blue, aIntensity, dIntensity, xPos, yPos, zPos, con, lin, exp)
{
	direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
	this->edge = edge;
	processedEdge = cosf(glm::radians(edge));
	isOn = true;
}

void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
	                     GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
	                     GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
                         GLuint edgeLocation)
{
	// Ambient
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	
	if (isOn)
	{
		// Ambient
		glUniform1f(ambientIntensityLocation, ambientIntensity);

		// Diffuse
		glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	}
	else
	{
		// Ambient
		glUniform1f(ambientIntensityLocation, 0.0f);

		// Diffuse
		glUniform1f(diffuseIntensityLocation, 0.0f);
	}

	// Position
	glUniform3f(positionLocation, position.x, position.y, position.z);

	// Direction
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);

	// Light Edge
	glUniform1f(edgeLocation, processedEdge);

	// Intensity factors
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}

void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir)
{
	position = pos;
	direction = dir;
}

SpotLight::~SpotLight()
{
}
