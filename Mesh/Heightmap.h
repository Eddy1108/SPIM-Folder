#pragma once
#include "Core/VisualObject.h"

#include "renderwindow.h"

class Heightmap : public VisualObject
{
public:

    Heightmap(std::string materialName, Texture* heightMapTexture);
	~Heightmap();
	void init() override;
	void draw() override;
	void constructMap();

	bool IsInside(glm::vec3 pos);

	float getHeight(glm::vec3 position);

protected:

	GLuint mEBO{ 0 };

    Texture* mTexture{nullptr};
    Texture* mHeightmap{nullptr};

	int mHeight{ 0 };
	int mWidth{ 0 };
	unsigned int mDetail{ 1 };
	float mScale{ 1 };
	float offsetX{ 0 };
	float offsetY{ 0 };

};

