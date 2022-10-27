#pragma once
#include "Core/VisualObject.h"

class TriangleSurface : public VisualObject
{
public:
	TriangleSurface(Shader& shader);
	TriangleSurface(std::string filename, Shader& shader);
	~TriangleSurface();

	void readFile(std::string filename);
	void init() override;
	void draw() override;
	void construct();
	void constructHeightmap();
	void writeFile();

	float func(float x, float y);

private:
	Texture* mTexture;
	Texture* mHeightMap;
};

