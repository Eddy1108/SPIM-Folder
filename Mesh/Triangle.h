#pragma once
#include "Core/VisualObject.h"

class Triangle : public VisualObject
{
public:
    Triangle(std::string materialName);
	~Triangle() override;
	void init() override;
	void draw() override;

private:
	Texture* mTexture;
};

