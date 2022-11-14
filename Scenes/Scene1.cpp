#include "Scene1.h"

Scene1::Scene1()
    : Scene()
{
    mCamera = new Camera();
}

Scene1::~Scene1()
{
}

void Scene1::objects()
{
    VisualObject* temp;
    //Untextured Objects
    mObjects.push_back(temp = new XYZ("materialplain"));
    temp->setName("XYZ");
    mObjects.push_back(temp = new InteractiveObject("materialplain"));
    temp->setName("mia");


    //Textured Objects
    mObjects.push_back(temp = new TriangleSurface("materialtexture"));
    temp->setName("TriangleSurface");


    for (auto it = mObjects.begin(); it != mObjects.end(); it++)
    {
        mMap.insert(std::pair<std::string, VisualObject*>{(*it)->getName(), * it});
    }

    //Make Quadtree
    Point2D a{ -20, -20 }, b{ 20,-20 }, c{ 20,20 }, d{ -20,20 };
    mQuadTre->init(a, b, c, d);
    mQuadTre->subDivide(3);
    for (auto it = mObjects.begin(); it != mObjects.end(); it++)
    {
        mQuadTre->insert((*it));
    }
}
