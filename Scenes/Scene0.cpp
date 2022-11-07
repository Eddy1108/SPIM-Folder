#include "Scene0.h"

#include "Mesh/ObjLoader.h"

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

    mObjects.push_back(temp = new ObjLoader(*mShaderPrograms["plain"], "../SPIM-Folder/Assets/models/crew.obj"));
    temp->setName("Jabbo");

    mObjects.push_back(temp = new Kube(*mShaderPrograms["plain"]));
    temp->setName("Kube");
    
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

void Scene0::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mCamera->mouseMovementStart(event->x(), event->y());
    }
}

void Scene0::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mCamera->mouseMovementStop();
    }
}

void Scene0::mouseMoveEvent(QMouseEvent *event)
{
    if (mCamera->MouseMove)
    {
        mCamera->mouseX = event->y();
        mCamera->mouseY = event->x();
    }
}

void Scene0::keyPressEvent(QKeyEvent *event)
{

    if(event->key() == Qt::Key_A)
    {
        if (!mCamera->bFollowPlayer)
        {
            mCamera->AMove = true;;
        }
        //else
        //    static_cast<InteractiveObject*>( mMap3["mia"])->AMove = true;

    }
    if (event->key() == Qt::Key_D)
    {

        if (!mCamera->bFollowPlayer)
        {
            mCamera->DMove = true;
        }
        //else
        //    static_cast<InteractiveObject*>( mMap3["mia"])->DMove = true;

    }

    if(event->key() == Qt::Key_S)
    {
        if (!mCamera->bFollowPlayer)
        {
            mCamera->SMove = true;
        }
        //else
        //    static_cast<InteractiveObject*>( mMap3["mia"])->SMove = true;

    }
    if (event->key() == Qt::Key_W)
    {
        if (!mCamera->bFollowPlayer)
        {
            mCamera->WMove = true;

        }
        //else
        //    static_cast<InteractiveObject*>( mMap3["mia"])->WMove = true;


    }
    if (event->key() == Qt::Key_G)
    {
        bPlayMode = !bPlayMode;
    }

    if (event->key() == Qt::Key_Q)
    {
        if (!mCamera->bFollowPlayer)
        {
            mCamera->QMove = true;
        }
        //else
        //    static_cast<InteractiveObject*>( mMap3["mia"])->QMove = true;
    }

    if (event->key() == Qt::Key_E)
    {
        if (!mCamera->bFollowPlayer)
        {
            mCamera->EMove = true;
        }
        //else
        //    static_cast<InteractiveObject*>( mMap3["mia"])->EMove = true;

    }
}

void Scene0::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_A)
    {
        if (!mCamera->bFollowPlayer)
        {
            mCamera->AMove = false;
        }
        else
            static_cast<InteractiveObject*>(mMap3["mia"])->AMove = false;

    }
    if (event->key() == Qt::Key_D)
    {
        if (!mCamera->bFollowPlayer)
        {
            mCamera->DMove = false;
        }
        else
            static_cast<InteractiveObject*>(mMap3["mia"])->DMove = false;

    }

    if (event->key() == Qt::Key_S)
    {
        if (!mCamera->bFollowPlayer)
        {
            mCamera->SMove = false;
        }
        else
            static_cast<InteractiveObject*>(mMap3["mia"])->SMove = false;

    }
    if (event->key() == Qt::Key_W)
    {
        if (!mCamera->bFollowPlayer)
        {
            mCamera->WMove = false;
        }
        else
        static_cast<InteractiveObject*>(mMap3["mia"])->WMove = false;

    }

    if (event->key() == Qt::Key_Q)
    {
        if (!mCamera->bFollowPlayer)
        {
            mCamera->QMove = false;
        }
        else
            static_cast<InteractiveObject*>(mMap3["mia"])->QMove = false;
    }

    if (event->key() == Qt::Key_E)
    {
        if (!mCamera->bFollowPlayer)
        {
             mCamera->EMove = false;
        }
        else
            static_cast<InteractiveObject*>(mMap3["mia"])->EMove = false;
    }
}
