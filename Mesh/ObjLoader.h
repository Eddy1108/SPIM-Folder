#pragma once
#include "Core/VisualObject.h"

#include <fstream>
#include <sstream>
#include <algorithm>

class ObjLoader : public VisualObject
{
public:
	ObjLoader(Shader& shader, const std::string objfiledir, std::string texfiledir);
	~ObjLoader();

	void init() override;
	void draw() override;

	void ReadFile(std::string filedir);

private:
	Texture* mTexture;
	bool bUseTex{ false };
};

