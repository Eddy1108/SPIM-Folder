#pragma once
#include "Light/Light.h"
#include "Mesh/ObjLoader.h"

class Sun : public Light
{
public:
	Sun(Shader& shader);

	void init() override;
	void draw() override;

private:
	ObjLoader* SunModel{ nullptr };
};

