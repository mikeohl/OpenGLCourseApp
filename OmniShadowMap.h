#pragma once
#include "ShadowMap.h"
class OmniShadowMap :
	public ShadowMap
{
public:
	OmniShadowMap();

	bool Init(unsigned int width, unsigned int height) override;

	void Write();

	void Read(GLenum TextureUnit);

	~OmniShadowMap();
};

