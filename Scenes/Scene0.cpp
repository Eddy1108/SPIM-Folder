#include "Scene0.h"

#include "Mesh/ObjLoader.h"
#include "Terrain/proceduralterrain.h"

#include "renderwindow.h"

Scene0::Scene0()
{
    mCamera = new Camera();
    RenderWindow::mCurrentCamera = mCamera;
    mCamera->mPosition += glm::vec3(0,0,20);

    mSkybox = new SkyBox("materialskybox");


    // ---- Particle Properties Example ----
    //mParticle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
    //mParticle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
    //mParticle.SizeBegin = 0.8f, mParticle.SizeVariation = 0.3f, mParticle.SizeEnd = 0.0f;
    //mParticle.LifeTime = 1.0f;
    //mParticle.Velocity = { 0.0f, 0.0f, 0.0f };
    //mParticle.VelocityVariation = { 3.0f, 3.0f, 3.0f };
    //mParticle.Position = { 0.0f, 0.0f, 0.0f };
    //mParticle.bFaceCamera = false;
    //mParticle.bUseGravity = true;
    //mParticle.bSizeOverTime = false;
    //mParticle.bColorOverTime = false;
    //mParticle.bTransparencyOverTime = true;

    mParticle.ColorBegin = { 1, 1, 1, 1.0f };
    mParticle.ColorEnd = { 244/255.0f, 211/255.0f, 35/255.0f, 0.0f };
    mParticle.SizeBegin = 0.8f, mParticle.SizeVariation = 0.3f, mParticle.SizeEnd = 0.0f;
    mParticle.LifeTime = 0.5f;
    mParticle.Velocity = { 0.0f, 0.0f, 0.0f };
    mParticle.VelocityVariation = { 8.0f, 8.0f, 8.0f };
    mParticle.Position = { 0.0f, 0.0f, 0.0f };
    mParticle.SpawnRate = 5;

    mParticle.bFaceCamera = false;
    mParticle.bUseGravity = true;
    mParticle.bSizeOverTime = false;
    mParticle.bColorOverTime = false;
    mParticle.bTransparencyOverTime = true;

}

Scene0::~Scene0()
{
}

void Scene0::objects()
{
    VisualObject* temp;

    ///Create Objects
    mObjects.push_back(temp = new XYZ("materialplain"));
    temp->setName("XYZ");

    mObjects.push_back(temp = new Kube("materialplain"));
    temp->setName("Kube");

    mObjects.push_back(temp = new ProceduralTerrain(mCamera, 25, "materialplain"));
    temp->setName("Terrain");

    
    mObjects.push_back(mParticleSystem = new ParticleSystem("materialparticle"));
    mParticleSystem->setName("ParticleSystem");

    InteractiveObject* mInteract = new Player("materialphong");
    //mObjects3.push_back(mInteract);
    mObjects.push_back(mInteract);
    mInteract->setName("mia");
    dynamic_cast<Player*>(mInteract)->mCam = mCamera;


    ///Dump it all into Unordered lists
    for (auto it = mObjects.begin(); it != mObjects.end(); it++)
    {
        mMap.insert(std::pair<std::string, VisualObject*>{(*it)->getName(), * it});
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
}

void Scene0::init()
{
    Scene::init();
}

void Scene0::draw()
{
 
    mParticleSystem->Spawn(mParticle);

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

    if (event->key() == Qt::Key_Shift)
    {
        if (!mCamera->bFollowPlayer)
        {
            mCamera->speed += 0.1;
        }
    }

    if (event->key() == Qt::Key_Control)
    {
        if (!mCamera->bFollowPlayer)
        {
            mCamera->speed -= 0.1;
        }
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
            static_cast<InteractiveObject*>(mMap["mia"])->AMove = false;

    }
    if (event->key() == Qt::Key_D)
    {
        if (!mCamera->bFollowPlayer)
        {
            mCamera->DMove = false;
        }
        else
            static_cast<InteractiveObject*>(mMap["mia"])->DMove = false;

    }

    if (event->key() == Qt::Key_S)
    {
        if (!mCamera->bFollowPlayer)
        {
            mCamera->SMove = false;
        }
        else
            static_cast<InteractiveObject*>(mMap["mia"])->SMove = false;

    }
    if (event->key() == Qt::Key_W)
    {
        if (!mCamera->bFollowPlayer)
        {
            mCamera->WMove = false;
        }
        else
        static_cast<InteractiveObject*>(mMap["mia"])->WMove = false;

    }

    if (event->key() == Qt::Key_Q)
    {
        if (!mCamera->bFollowPlayer)
        {
            mCamera->QMove = false;
        }
        else
            static_cast<InteractiveObject*>(mMap["mia"])->QMove = false;
    }

    if (event->key() == Qt::Key_E)
    {
        if (!mCamera->bFollowPlayer)
        {
             mCamera->EMove = false;
        }
        else
            static_cast<InteractiveObject*>(mMap["mia"])->EMove = false;
    }
}
