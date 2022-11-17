#include "MaterialBillboard.h"

MaterialBillboard::MaterialBillboard(GLuint shaderProgram, int textureIndex) : Material(shaderProgram, textureIndex)
{

}

void MaterialBillboard::UpdateUniforms(glm::mat4* mMatrix)
{
    initializeOpenGLFunctions();
    glUseProgram(mShaderProgram);

    //Update Uniform variables
    glUniformMatrix4fv(mUniform.find("mMatrix")->second, 1, GL_FALSE, glm::value_ptr(*mMatrix));
    glUniform1i(mUniform.find("textureSampler")->second, mTextureIndex[0]);
}

void MaterialBillboard::SetupUniforms()
{
    initializeOpenGLFunctions();
    if (uniformsHasBeenSetup) return;

    mUniform.insert(std::make_pair<std::string, GLuint>("mMatrix", glGetUniformLocation(mShaderProgram, "mMatrix")));
    mUniform.insert(std::make_pair<std::string, GLuint>("vMatrix", glGetUniformLocation(mShaderProgram, "vMatrix")));
    mUniform.insert(std::make_pair<std::string, GLuint>("pMatrix", glGetUniformLocation(mShaderProgram, "pMatrix")));
    mUniform.insert(std::make_pair<std::string, GLuint>("textureSampler", glGetUniformLocation(mShaderProgram, "textureSampler")));

    uniformsHasBeenSetup = true;
}
