#pragma once
#include "Scene.h"

#include "mesh/OctahedronBall.h"


class Scene1 : public Scene
{
public:
	Scene1(std::unordered_map<std::string, Shader*> shaders);
	~Scene1();

	//void init() override;
	//void draw() override;


};

