#include "line.h"

Line::Line(const glm::vec3& direction, const glm::mat4& positionMatrix, glm::vec3 color, Shader& shader) : VisualObject(shader)
{
    mMatrix = glm::mat4(1.0f);

    mDirection = &direction;
    mObjPosition = &positionMatrix;

    mVertices.push_back(Vertex{0,0,0,color.x, color.y, color.z});
    mVertices.push_back(Vertex{1,0,0.1,color.x, color.y, color.z});
}

void Line::init()
{
    //mMatrixUniform = glGetUniformLocation( mShaderProgram->getProgram(), "matrix" );

    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );
}

void Line::draw()
{
    // Ikke optimalisert, burde heller rotere
    mVertices.at(1).m_xyz[0] = mDirection->x; mVertices.at(1).m_xyz[1] = mDirection->y; mVertices.at(1).m_xyz[2] = mDirection->z;

    //mPosition = *mObjPosition;
    //mMatrix = mPosition * mScale;

    mMatrix = *mObjPosition * mScale;

    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, glm::value_ptr(mMatrix));
    glDrawArrays(GL_LINES, 0, mVertices.size());
}

void Line::scale(float s)
{
    //mScale.scale(s, s, s);
    mScale = glm::scale(mScale, glm::vec3(s, s, s));
}
