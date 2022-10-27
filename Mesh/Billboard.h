#pragma once
#include "Core/VisualObject.h"
#include "Core/Camera.h"
#include "Core/texture.h"


class Billboard : public VisualObject
{
public:

	Billboard(Shader& shader, Camera* cam);
	Billboard(Shader& shader, Camera* cam, std::string fileDir);
	~Billboard();

	void init() override;
	void draw() override;

	void move();

	glm::vec3 mUp = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 mOrientation = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 billView = glm::mat4(1.0f);

private:
	Camera* mCam{nullptr};

	Texture* mTexture{ nullptr };

	void RotateToCamMatrix();
	void RotateToCamDirect();
};

