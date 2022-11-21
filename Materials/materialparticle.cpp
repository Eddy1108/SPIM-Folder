#include "materialparticle.h"
#include "renderwindow.h"

MaterialParticle::MaterialParticle(GLuint shaderProgram, int textureIndex)
    : Material(shaderProgram, textureIndex)
{

}

void MaterialParticle::UpdateUniforms()
{
    initializeOpenGLFunctions();
    glUseProgram(mShaderProgram);

    //Update Uniform variables
    //glUniformMatrix4fv( mUniform.find("mMatrix")->second, 1, GL_FALSE, glm::value_ptr(*mMatrix));
    glUniform1i(mUniform.find("textureSampler")->second, mTextureIndex[0]);
    if (RenderWindow::mCurrentCamera)
    {
        glUniform3fv(mUniform.find("CameraRight_worldspace")->second, 1, glm::value_ptr(RenderWindow::mCurrentCamera->GetRight()));
        glUniform3fv(mUniform.find("CameraUp_worldspace")->second, 1, glm::value_ptr(RenderWindow::mCurrentCamera->GetUp()));
    }
    else {
        std::cout << "CAMERA DONT EXIST!!!!!!!!!!!!" << std::endl;
    }

}

void MaterialParticle::SetupUniforms()
{
    initializeOpenGLFunctions();
    if(uniformsHasBeenSetup) return;

    mUniform.insert(std::make_pair<std::string,GLuint>("vMatrix", glGetUniformLocation( mShaderProgram, "vMatrix")));
    mUniform.insert(std::make_pair<std::string,GLuint>("pMatrix", glGetUniformLocation( mShaderProgram, "pMatrix")));
    mUniform.insert(std::make_pair<std::string,GLuint>("textureSampler", glGetUniformLocation( mShaderProgram, "textureSampler")));
    mUniform.insert(std::make_pair<std::string, GLuint>("CameraRight_worldspace", glGetUniformLocation(mShaderProgram, "CameraRight_worldspace")));
    mUniform.insert(std::make_pair<std::string, GLuint>("CameraUp_worldspace", glGetUniformLocation(mShaderProgram, "CameraUp_worldspace")));


    uniformsHasBeenSetup = true;
}
