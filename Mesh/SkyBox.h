#pragma once
#include "Core/VisualObject.h"


class SkyBox : public VisualObject
{
public:
	SkyBox(Shader& shader);

	void init() override;

	void draw() override;

private:

	GLuint cubemapTexture;

	std::vector<std::string> CubemapDir;

};

