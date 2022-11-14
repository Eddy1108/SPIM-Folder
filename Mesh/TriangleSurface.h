#pragma once
#include "Core/VisualObject.h"

class TriangleSurface : public VisualObject
{
public:
    TriangleSurface(std::string materialName);
    TriangleSurface(std::string filename, std::string materialName);
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

