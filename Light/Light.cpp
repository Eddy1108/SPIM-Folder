#include "light.h"

Light::Light(Shader& shader)
    : VisualObject(shader)
{
    mVertices.insert(mVertices.end(),
        {
         Vertex{-0.25f, -0.25f,  0.25f, 0.8f, 0.8f, 0.3f, 0.f,  0.f},   //Left low
         Vertex{0.25f, -0.25f,  0.25f, 0.8f, 0.8f, 0.3f,  1.f,  0.f},   //Right low
         Vertex{0.0f,  0.25f,  0.0f, 0.8f, 0.8f, 0.3f, 0.5f, 0.5f},  //Top
         Vertex{0.0f, -0.25f, -0.25f, 0.8f, 0.8f, 0.3f, 0.5f, 0.5f}   //Back low
        });

    mIndices.insert(mIndices.end(),
        { 0, 1, 2,
          1, 3, 2,
          3, 0, 2,
          0, 3, 1
        });

    mMatrix = glm::mat4(1.0f);
}

Light::~Light()
{
}

void Light::init()
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

    mMatrix = glm::mat4(1.0f);
    move(0.f, 0.f, 20.f);
}

void Light::draw()
{
    //mMatrix = glm::mat4(1.0f);
    glm::vec3 moveStep = glm::vec3(0.06f, 0.06f, 0.f);
    mMatrix = glm::translate(mMatrix, moveStep);
    mPosition += moveStep;

    glBindVertexArray(mVAO);
    glUniformMatrix4fv(mShader.mMatrixUniform, 1, GL_FALSE, glm::value_ptr(mMatrix));
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
