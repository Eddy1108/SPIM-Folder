#include "Scene.h"

#include "renderwindow.h"
#include "Core/materiallist.h"

Scene::Scene()
{
    mQuadTre = new QuadTree();
}

Scene::~Scene()
{
    //Delete all objects
    for(auto it = mMap.begin(); it != mMap.end(); it++)
        delete (*it).second;
}

void Scene::init()
{
    //Construct objects
    objects();

    //Initlaize skybox
    if (mSkybox)
    {
        //Skybox
        //mSkybox->setMaterial("materialcubemap"); // not needed, since it's done vObject
        mSkybox->init();
    }
    
    //Initialize objects within mMap
    for(auto it = mMap.begin(); it != mMap.end(); it++)
        (*it).second->init();
}

void Scene::draw()
{
//    initializeOpenGLFunctions();

    PauseObjects();

    //BEGIN
    mCamera->perspective(60, (float)16 / (float)9, 0.1f, 5000.0);
    //mCamera->perspective(60, width/height, 0.1, 3000.0);

    //**********Draws all the elements in the scene**********
    //Skybox
    if(mSkybox){
        mSkybox->draw();
    }

//    //Draw Plain Objects
    if (mMap.size() > 0 || mSkybox)
    {
        //Move Player
        mMap["mia"]->move(0.1f, 0.1f, 0.1f);   //old dumb way of doint it but gonna let it stay, it works


     //   GLint tempPMatrix = mShaderPrograms["plain"]->mPMatrixUniform;
     //   GLint tempVMatrix = mShaderPrograms["plain"]->mVMatrixUniform;

        // Camera
       // mCamera->init(tempPMatrix, tempVMatrix);
//        mCamera->update();

        mCamera->lookAt(
            mMap["mia"]->mCameraOffset,
            mMap["mia"]->getPosition3D(),
            glm::vec3{ 0.0f, 0.0f, 1.0f }
        );

        for(int i = 0; i < RenderWindow::mMaterialList->mMaterialSize; i++){
            if(!RenderWindow::mMaterialList->mMaterial[i].first.empty()){
                RenderWindow::mMaterialList->mMaterial[i].second->UpdateCameraUniform(&mCamera->mVMatrix, &mCamera->mPMatrix);
            }
            else break;
        }


        //Move Camera, look at player


        // Draw objects
        for (auto it = mMap.begin(); it != mMap.end(); it++)
        {
            (*it).second->draw();
        }
    }

//    //Draw Texture Objects
//    if (mMap2.size() > 0)
//    {
//        GLint tempPMatrix = mShaderPrograms["textured"]->mPMatrixUniform;
//        GLint tempVMatrix = mShaderPrograms["textured"]->mVMatrixUniform;
//        glUseProgram(mShaderPrograms["textured"]->getProgram());

//        glUniform1i(mShaderPrograms["textured"]->mTextureUniform, 1);

//        mCamera->init(tempPMatrix, tempVMatrix);
//        mCamera->update();

//        //Move Camera
//        mCamera->lookAt(
//            mMap3["mia"]->mCameraOffset,
//            mMap3["mia"]->getPosition3D(),
//            glm::vec3{ 0.0f, 0.0f, 1.0f }
//        );

//        for (auto it = mMap2.begin(); it != mMap2.end(); it++)
//        {
//            (*it).second->draw();
//        }
//    }

//    //Phong Objects
//    if (mMap3.size() > 0)
//    {
//        GLint tempPMatrix = mShaderPrograms["phong"]->mPMatrixUniform;
//        GLint tempVMatrix = mShaderPrograms["phong"]->mVMatrixUniform;
//        glUseProgram(mShaderPrograms["phong"]->getProgram());

//        glUniform1i(mShaderPrograms["phong"]->mTextureUniform, 1);

//        //Update light
//        glUniform3f(mShaderPrograms["phong"]->mCameraPositionUniform, mCamera->mPosition.x, mCamera->mPosition.y, mCamera->mPosition.z);
//        //glUniform3f(mShaderPrograms["phong"]->mLightPositionUniform, mLight->getPosition3D().x, mLight->getPosition3D().y, mLight->getPosition3D().z);
//        glUniform3f(mShaderPrograms["phong"]->mLightPositionUniform, 0.f, 0.f, 0.f);

//        mCamera->init(tempPMatrix, tempVMatrix);
//        mCamera->update();


//        //Move Camera
//        mCamera->lookAt(
//            mMap3["mia"]->mCameraOffset,
//            mMap3["mia"]->getPosition3D(),
//            glm::vec3{ 0.0f, 0.0f, 1.0f }
//        );
        

//        for (auto it = mMap3.begin(); it != mMap3.end(); it++)
//        {
//            (*it).second->draw();
//        }
//    }

    checkCollision();

    drawCollision();

}

void Scene::drawCollision()
{
    //Draw collision for Plain objects
    for (auto it = mMap.begin(); it != mMap.end(); it++)
    {
        if ((*it).second->mBShape)
        {
            if ((*it).second->mBShape->mType == CollisionShape::Type::AABB)
            {

                if ((*it).second->mBShape->bDrawBox)
                {
                    DrawAABB(
                        dynamic_cast<AABB*>((*it).second->mBShape)->mPosition,
                        dynamic_cast<AABB*>((*it).second->mBShape)->mExtent
                    );
                }
            }
        }
    }

    DrawAABB(mQuadTre->m_ne->mShape->mPosition, mQuadTre->m_ne->mShape->mExtent);
    DrawAABB(mQuadTre->m_nw->mShape->mPosition, mQuadTre->m_nw->mShape->mExtent);
    DrawAABB(mQuadTre->m_se->mShape->mPosition, mQuadTre->m_se->mShape->mExtent);
    DrawAABB(mQuadTre->m_sw->mShape->mPosition, mQuadTre->m_sw->mShape->mExtent);
}

void Scene::checkCollision()
{
    //glm::vec2 playerPos = mMap3["mia"]->getPosition2D();
    //glm::vec2 NPCPos = mMap3["NPC"]->getPosition2D();

    //Check Collision for Player
    //QuadTre* subtre = mQuadTre->find(playerPos);
    //for (auto it = subtre->m_Objects.begin(); it < subtre->m_Objects.end(); it++)
    //{
    //    if ((*it)->mBShape && dynamic_cast<Trophy*>(*it) != nullptr && dynamic_cast<Trophy*>(*it)->bIsRed && mMap3["mia"]->mBShape->overlap((*it)->mBShape))
    //    {
    //        
    //        if ((*it)->activate())
    //        {
    //            dynamic_cast<Player*>(mMap3["mia"])->CollectTrophy();
    //        }
    //        
    //    }

    //    //if ((*it)->mBShape && dynamic_cast<Fence*>(*it) != nullptr && mMap3["mia"]->mBShape->overlap((*it)->mBShape))
    //    //{
    //    //    dynamic_cast<Player*>(mMap3["mia"])->bBlocked = true;
    //    //}
    //}

    ////Check Collision for NPC
    //subtre = mQuadTre->find(NPCPos);
    //for (auto it = subtre->m_Objects.begin(); it < subtre->m_Objects.end(); it++)
    //{
    //    if ((*it)->mBShape && dynamic_cast<Trophy*>(*it) != nullptr && !(dynamic_cast<Trophy*>(*it)->bIsRed) && mMap3["NPC"]->mBShape->overlap((*it)->mBShape))
    //    {

    //        if ((*it)->activate())
    //        {
    //            if ((dynamic_cast<NPC*>(mMap3["NPC"])->CollectTrophy())) 
    //            {
    //                std::cout << "ENEMY WIN" << std::endl;
    //                dynamic_cast<Player*>(mMap3["mia"])->WinState = 2;
    //            }

    //        }

    //    }
    //}

    //Check Collision for BOMB

}

void Scene::objects()
{
    //mMap.push_back(...)
}

void Scene::DrawAABB(glm::vec3 position, glm::vec3 extent)
{
//    static uint32_t VAO{};
//    if (VAO == 0)
//    {
//        std::vector<Vertex> verts;
//        float size{ 1.f };
//        verts.push_back(Vertex{ -size,  size, -size, 1,1,1 });
//        verts.push_back(Vertex{ size,  size, -size, 1,1,1 });
//        verts.push_back(Vertex{ size,  size,  size, 1,1,1 });

//        verts.push_back(Vertex{ -size,  size,  size, 1,1,1 });
//        verts.push_back(Vertex{ -size, -size, -size, 1,1,1 });
//        verts.push_back(Vertex{ size, -size, -size, 1,1,1 });
//        verts.push_back(Vertex{ size, -size,  size, 1,1,1 });
//        verts.push_back(Vertex{ -size, -size,  size, 1,1,1 });

//        //AABB bounding box vertex indices
//        std::vector<GLuint> indices{ 0,1,1,2,2,3,3,0,4,5,5,6,6,7,7,4,0,4,1,5,2,6,3,7 };

//        uint32_t VBO{};
//        uint32_t EAB{};

//        initializeOpenGLFunctions();
//        //Vertex Array Object - VAO
//        glGenVertexArrays(1, &VAO);
//        glBindVertexArray(VAO);

//        //Vertex Buffer Object to hold vertices - VBO
//        glGenBuffers(1, &VBO);
//        glBindBuffer(GL_ARRAY_BUFFER, VBO);

//        glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_STATIC_DRAW);

//        // 1rst attribute buffer : vertices
//        glBindBuffer(GL_ARRAY_BUFFER, VBO);
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
//        glEnableVertexAttribArray(0);

//        // 2nd attribute buffer : colors
//        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
//        glEnableVertexAttribArray(1);

//        //Second buffer - holds the indices (Element Array Buffer - EAB):
//        glGenBuffers(1, &EAB);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAB);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
//    }

//    glm::mat4 matrix(1.f);
//    matrix[3].x = position.x;
//    matrix[3].y = position.y;
//    matrix[3].z = position.z;

//    matrix[0].x = extent.x;
//    matrix[1].y = extent.y;
//    matrix[2].z = extent.z;

//    //GLint prog = 0;
//    //glGetIntegerv(GL_CURRENT_PROGRAM, &prog);

//    glUseProgram(mShaderPrograms["plain"]->getProgram());

//    glBindVertexArray(VAO);
//    glUniformMatrix4fv(mShaderPrograms["plain"]->mMatrixUniform,          //the location of the matrix in the shader
//        1,                       //count
//        GL_FALSE,                //transpose the matrix before sending it?
//        glm::value_ptr(matrix));    //the data of the matrix

//    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

//    glBindVertexArray(0);

//    //glUseProgram(prog);

}

void Scene::PauseObjects()
{
        mCamera->bFollowPlayer = bPlayMode;

    if (dynamic_cast<Player*>(mMap["mia"]))
        dynamic_cast<Player*>(mMap["mia"])->bDrawCam = !bPlayMode;

    if (dynamic_cast<XYZ*>(mMap["XYZ"]))
        dynamic_cast<XYZ*>(mMap["XYZ"])->bDraw = !bPlayMode;

}
