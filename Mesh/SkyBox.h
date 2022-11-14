#pragma once
#include "Core/VisualObject.h"


class SkyBox : public VisualObject
{
public:
    SkyBox(std::string materialName);

	void init() override;

	void draw() override;

private:

	GLuint cubemapTexture;

	std::vector<std::string> CubemapDir;

};

