#pragma once
#include "Core/VisualObject.h"

class Triangle : public VisualObject
{
public:
	Triangle(Shader& shader);
	~Triangle() override;
	void init() override;
	void draw() override;

private:
	Texture* mTexture;
};

