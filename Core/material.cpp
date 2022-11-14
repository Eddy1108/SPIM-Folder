#include "material.h"
#include <iostream>
#include "glm/gtc/type_ptr.hpp"

Material::Material(GLuint shaderProgram)
    : mShaderProgram(shaderProgram)
{
    initializeOpenGLFunctions();
}

Material::Material(GLuint shaderProgram, int textureIndex)
    : mShaderProgram(shaderProgram)
{
    mTextureIndex.push_back(textureIndex);
    initializeOpenGLFunctions();
}

void Material::Init()
{

}

void Material::UpdateCameraUniform(glm::mat4 *vMatrix, glm::mat4 *pMatrix)
{
    initializeOpenGLFunctions();
    glUseProgram(mShaderProgram);

    glUniformMatrix4fv(mUniform.find("vMatrix")->second, 1, GL_FALSE, glm::value_ptr(*vMatrix));
    glUniformMatrix4fv(mUniform.find("pMatrix")->second, 1, GL_FALSE, glm::value_ptr(*pMatrix));
}

GLuint Material::GetUniform(std::string uniformName)
{
    //if(!mUniform[uniformName])
        //qDebug() << "Error, uniformName doesn't exist.| material.cpp, line 30";
    return mUniform.find(uniformName)->second;
}

