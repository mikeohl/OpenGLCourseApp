#include "ShadowMap.h"



ShadowMap::ShadowMap()
{
	FBO = 0;
	shadowMap = 0;
}


bool ShadowMap::Init(GLuint width, GLuint height)
{
	shadowWidth = width;
	shadowHeight = height;

	glGenFramebuffers(1, &FBO);

	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	
	// Set parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// Set the border to be all the way to the far plane (e.g. nothing appears in front and so it is not in shadow)
	float bColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bColor);
	
	// Bind the buffer and set the texture
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);

	// Don't read from color attachments
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Check for framebuffer error
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer Error: %i\n", status);
		return false;
	}

	// Unbind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}


void ShadowMap::Write()
{
	// Bind the buffer 
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}


void ShadowMap::Read(GLenum textureUnit)
{
	// Set the active texture unit and bind the shadowMap to it 
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
}


ShadowMap::~ShadowMap()
{
	if (FBO)
	{
		glDeleteFramebuffers(1, &FBO);
	}

	if (shadowMap)
	{
		glDeleteTextures(1, &shadowMap);
	}
}
