#include "materialplain.h"

MaterialPlain::MaterialPlain(GLuint shaderProgram) : Material(shaderProgram)
{
}

void MaterialPlain::UpdateUniforms(glm::mat4* mMatrix)
{
    initializeOpenGLFunctions();
    glUseProgram(mShaderProgram);

    //Update Uniform variables
    glUniformMatrix4fv( mUniform.find("mMatrix")->second, 1, GL_FALSE, glm::value_ptr(*mMatrix));
}

void MaterialPlain::SetupUniforms()
{
    initializeOpenGLFunctions();
    if(uniformsHasBeenSetup) return;

    mUniform.insert(std::make_pair<std::string,GLuint>("mMatrix", glGetUniformLocation( mShaderProgram, "mMatrix")));
    mUniform.insert(std::make_pair<std::string,GLuint>("vMatrix", glGetUniformLocation( mShaderProgram, "vMatrix")));
    mUniform.insert(std::make_pair<std::string,GLuint>("pMatrix", glGetUniformLocation( mShaderProgram, "pMatrix")));

    uniformsHasBeenSetup = true;
}
