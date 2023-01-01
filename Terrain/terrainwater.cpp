#include "terrainwater.h"

TerrainWater::TerrainWater(float waterHeightOffset, glm::vec2 position, const float chunkSize, std::string materialName) : VisualObject(materialName)
{
    generateWater(waterHeightOffset, position, chunkSize);
}

void TerrainWater::generateWater(float waterHeightOffset, glm::vec2 pos, float chunkSize)
{
    //Create flat water surface
    mVertices.push_back(Vertex{pos.x, pos.y,waterHeightOffset, 0.1f,0.3f,1, 0,0});
    mVertices.push_back(Vertex{pos.x + chunkSize, pos.y,waterHeightOffset, 0.1f,0.3f,1, 1,0});
    mVertices.push_back(Vertex{pos.x + chunkSize, pos.y + chunkSize,waterHeightOffset, 0.1f,0.3f,1, 1,1});
    mVertices.push_back(Vertex{pos.x, pos.y + chunkSize,waterHeightOffset, 0.1f,0.3f,1, 0,1});

    mIndices.push_back(0);
    mIndices.push_back(1);
    mIndices.push_back(2);
    mIndices.push_back(0);
    mIndices.push_back(2);
    mIndices.push_back(3);
}

void TerrainWater::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER,      //what buffer type
        mVertices.size() * sizeof(Vertex),			   //how big buffer do we need
        mVertices.data(),             //the actual vertices
        GL_STATIC_DRAW				 //should the buffer be updated on the GPU
    );

    //EBO
    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);


    // 1st attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match layout(location = 0) in the vertex shader.
        3,                  // size / number of elements of data type
        GL_FLOAT,           // data type
        GL_FALSE,           // normalize data
        sizeof(Vertex),  // stride
        reinterpret_cast<GLvoid*>(0));          // array buffer offset
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    // Same parameter list as above but attribute and offset is adjusted accoringly
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : UV
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);	//release
}

void TerrainWater::draw()
{
    mMaterial->UpdateUniforms(&mMatrix);

    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
