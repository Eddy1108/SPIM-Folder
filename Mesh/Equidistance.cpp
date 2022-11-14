#include "Mesh/Equidistance.h"

Equidistance::Equidistance(std::string materialName) : VisualObject(materialName)
{
    mMatrix1.setToIdentity();
}

Equidistance::Equidistance(std::string materialName, QVector3D a, QVector3D b) : VisualObject(materialName)
{

    mVertices.push_back(Vertex{ a.x(), a.y(), a.z(),1,1,1 });
    mVertices.push_back(Vertex{ b.x(), b.y(), b.z(),1,1,1 });

    mMatrix1.setToIdentity();
}

void Equidistance::insertLine(QVector3D a, QVector3D b)
{
    mVertices.push_back(Vertex{ a.x(), a.y(), a.z(),1,1,1 });
    mVertices.push_back(Vertex{ b.x(), b.y(), b.z(),1,1,1 });
}

void Equidistance::init()
{
    initializeOpenGLFunctions();
    //Maybe move location later
    mMatrix1.setToIdentity();

    //Vertex Array Object - VAO
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

    // 1st attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Equidistance::draw()
{
    mMaterial->UpdateUniforms(&mMatrix);
    glBindVertexArray(mVAO);
    glDrawArrays(GL_LINES, 0, mVertices.size());
    glBindVertexArray(0);
}
