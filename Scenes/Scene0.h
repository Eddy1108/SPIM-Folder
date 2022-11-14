#pragma once
#include "Scene.h"

#include "mesh/Kube.h"
//#include "mesh/Graph.h"
//#include "mesh/OctahedronBall.h"
//#include "mesh/Disc.h"
//#include "Math/BezierCurve.h"

//#include "mesh/Triangle.h"
#include "Light/Light.h"
//#include "mesh/Heightmap.h"
//#include "Mesh/Billboard.h"
#include "Mesh/Player/Player.h"

#include "Mesh/ObjLoader.h"
#include "Mesh/SkyBox.h"

//#include "Mesh/PointCloud.h"
//#include "Mesh/LAZSurface.h"
//#include "Mesh/rollingball.h"

class Scene0 : public Scene
{

public:
    Scene0();
	~Scene0();

    void objects() override;
	void init() override;
    void draw() override;

    //Input Events
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
};

