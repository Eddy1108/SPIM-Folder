#include "Scene1.h"

Scene1::Scene1(std::unordered_map<std::string, Shader*> shaders)
    : Scene(shaders)
{
    mCamera = new Camera();

    VisualObject* temp;

    //Untextured Objects
    mObjects.push_back(temp = new XYZ(*mShaderPrograms["plain"]));
    temp->setName("XYZ");
    mObjects.push_back(temp = new InteractiveObject(*mShaderPrograms["plain"]));
    temp->setName("mia");


    //Textured Objects
    mObjects2.push_back(temp = new TriangleSurface(*mShaderPrograms["textured"]));
    temp->setName("TriangleSurface");


    for (auto it = mObjects.begin(); it != mObjects.end(); it++)
    {
        mMap.insert(std::pair<std::string, VisualObject*>{(*it)->getName(), * it});
    }
    for (auto it = mObjects2.begin(); it != mObjects2.end(); it++)
    {
        mMap2.insert(std::pair<std::string, VisualObject*>{(*it)->getName(), * it});
    }

    //Make Quadtree
    Point2D a{ -20, -20 }, b{ 20,-20 }, c{ 20,20 }, d{ -20,20 };
    mQuadTre->init(a, b, c, d);
    mQuadTre->subDivide(3);
    for (auto it = mObjects.begin(); it != mObjects.end(); it++)
    {
        mQuadTre->insert((*it));
    }
    for (auto it = mObjects2.begin(); it != mObjects2.end(); it++)
    {
        mQuadTre->insert((*it));
    }
}

Scene1::~Scene1()
{
}
