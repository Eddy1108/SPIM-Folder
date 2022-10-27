#pragma once
#include "Scene.h"

#include "mesh/Kube.h"
#include "mesh/Graph.h"
#include "mesh/OctahedronBall.h"
#include "mesh/Disc.h"
#include "Math/BezierCurve.h"


#include "mesh/Triangle.h"
#include "Light/Light.h"
#include "Light/Sun.h"
#include "mesh/Heightmap.h"
#include "Mesh/Billboard.h"
#include "Mesh/Player/Player.h"

#include "Mesh/ObjLoader.h"
#include "Mesh/SkyBox.h"


#include "Mesh/PointCloud.h"
#include "Mesh/LAZSurface.h"
#include "Mesh/rollingball.h"
#include "Mesh/ballspawner.h"

class Scene0 : public Scene
{

public:
	Scene0(std::unordered_map<std::string, Shader*> shaders);
	~Scene0();

	void init() override;
	void draw() override;

	PointCloud* mCloud{ nullptr };
	static void SpawnMoreBalls(int amount);
	static inline BallSpawner* mBallSpawner{ nullptr };
};

