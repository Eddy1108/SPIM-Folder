#include "Scene0.h"

Scene0::Scene0(std::unordered_map<std::string, Shader*> shaders)
    : Scene(shaders)
{
    mCamera = new Camera();

    mSkybox = new SkyBox(*mShaderPrograms["skybox"]);

    VisualObject* temp;


    ///Create Objects
        //Untextured Objects
    mObjects.push_back(temp = new XYZ(*mShaderPrograms["plain"]));
    temp->setName("XYZ");

    mLight = new Sun(*mShaderPrograms["plain"]);
    mObjects.push_back(mLight);
    mLight->setName("Sun");

    mCloud = new PointCloud(*mShaderPrograms["plain"]);
    mObjects.push_back(mCloud);

    LAZSurface* surface = new LAZSurface("../SPIM-Folder/Surface/GlitterholetShortened.txt", mCloud, QVector2D(600,300), *mShaderPrograms["plain"], QVector3D(-473213.f - 1110 / 2, -6835647.f - 2110 / 2, -1734.f));
    mObjects.push_back(surface);
    surface->setName("LAZSurface");

    temp = new RollingBall(3, QVector3D(0,0, 100), *mShaderPrograms["plain"], surface);
    mObjects.push_back(temp);
    temp->setName("RollingBall");

    mBallSpawner = new BallSpawner(*mShaderPrograms["plain"], surface);
    mObjects.push_back(mBallSpawner);
    mBallSpawner->setName("BallSpawner");
    

    ///Textured Objects
//Use mObjects2!



    ///Phong Objects (+ Texture)
//Use mObjects3!

  
    InteractiveObject* mInteract = new Player(*mShaderPrograms["phong"]);
    mObjects3.push_back(mInteract);
    mInteract->setName("mia");
    dynamic_cast<Player*>(mInteract)->mCam = mCamera;

    ///Dump it all into Unordered lists
    for (auto it = mObjects.begin(); it != mObjects.end(); it++)
    {
        mMap.insert(std::pair<std::string, VisualObject*>{(*it)->getName(), * it});
    }

    for (auto it = mObjects2.begin(); it != mObjects2.end(); it++)
    {
        mMap2.insert(std::pair<std::string, VisualObject*>{(*it)->getName(), * it});
    }

    for (auto it = mObjects3.begin(); it != mObjects3.end(); it++)
    {
        mMap3.insert(std::pair<std::string, VisualObject*>{(*it)->getName(), * it});
    }


    //Make Quadtree
    Point2D a{ -40, -40 }, b{ 40,-40 }, c{ 40,40 }, d{ -40,40 };
    mQuadTre->init(a, b, c, d);
    mQuadTre->subDivide(2);
    for (auto it = mObjects.begin(); it != mObjects.end(); it++)
    {
        if ((*it)->mBShape)
        {
            mQuadTre->insert((*it), (*it)->mBShape);    //Insert objects with a collision shape into the quadtre
        }

    }

    for (auto it = mObjects3.begin(); it != mObjects3.end(); it++)
    {
        if ((*it)->mBShape)
        {
            mQuadTre->insert((*it), (*it)->mBShape);
        }
    }
}

Scene0::~Scene0()
{
}

void Scene0::init()
{
    Scene::init();
}

void Scene0::draw()
{
    Scene::draw();

}

void Scene0::SpawnMoreBalls(int amount)
{
    mBallSpawner->SpawnBalls(amount);
}
