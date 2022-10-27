#include "Sun.h"

Sun::Sun(Shader& shader)
	:Light(shader) 
{
	SunModel = new ObjLoader(shader, "../3DProgExam/Assets/models/Sun.obj", "");

    //Make the model Yellow
    for (int i = 0; i < SunModel->mVertices.size(); i++)
    {
        SunModel->mVertices[i].m_normal[0] = 0.9;
        SunModel->mVertices[i].m_normal[1] = 0.9;
        SunModel->mVertices[i].m_normal[2] = 0.3;
    }

    mVertices = SunModel->mVertices;
    mIndices = SunModel->mIndices;


    //None of these do anything, these settings are changed in the frag shader itself for now
    mAmbientStrenght = 0.0f;
    mAmbientColor = glm::vec3{ 0.0f, 0.0f, 0.0f };

    mLightStrenght =  0.9f;
    mLightColor = glm::vec3{ 0.9f, 0.9f, 0.3f };    //Yellow light color

    mSpecularStrenght = 0.9f ;
    mSpecularExponent = 4;
}

void Sun::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    SunModel->init();

    mMatrix = glm::mat4(1.0f);
    move(10.f, 10.f, 20.f);
}

void Sun::draw()
{
//    if (bPlay)
//    {
//        glm::vec3 rotDistance{ 10.f,10.f,10.f };
//
//        //Rotate Sun
//        mMatrix = glm::translate(mMatrix, -rotDistance);
//        mMatrix = glm::rotate(mMatrix, glm::radians(-0.5f), glm::vec3{ 0.f,0.f,1.f });
//        mMatrix = glm::translate(mMatrix, rotDistance);
//
//        mPosition = glm::vec3{ mMatrix[3].x, mMatrix[3].y, mMatrix[3].z };
//
//    }
//
//    SunModel->mMatrix = mMatrix;
//    SunModel->draw();
//
//    glBindVertexArray(mVAO);
//    glUniformMatrix4fv(mShader.mMatrixUniform, 1, GL_FALSE, glm::value_ptr(mMatrix));
//    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
//    glBindVertexArray(0);
}
