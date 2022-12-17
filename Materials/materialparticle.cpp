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
    if (RenderWindow::mCurrentCamera)
    {
        glm::mat4 ViewProj(1.0f);
        ViewProj = RenderWindow::mCurrentCamera->mVMatrix * RenderWindow::mCurrentCamera->mPMatrix;

        //glUniformMatrix4fv(mUniform.find("uViewProj")->second, 1, GL_FALSE, glm::value_ptr(ViewProj));
    }

}

void MaterialParticle::UpdateUniforms(glm::mat4* Transform, glm::vec4* color)
{
    initializeOpenGLFunctions();
    glUseProgram(mShaderProgram);

    glUniformMatrix4fv(mUniform.find("uTransform")->second, 1, GL_FALSE, glm::value_ptr(*Transform));
    glUniform4fv(mUniform.find("uColor")->second, 1, glm::value_ptr(*color));

    glUniform1i(mUniform.find("textureSampler")->second, mTextureIndex[0]);
}

void MaterialParticle::SetupUniforms()
{
    initializeOpenGLFunctions();
    if (uniformsHasBeenSetup) return;

    mUniform.insert(std::make_pair<std::string, GLuint>("vMatrix", glGetUniformLocation(mShaderProgram, "vMatrix")));
    mUniform.insert(std::make_pair<std::string, GLuint>("pMatrix", glGetUniformLocation(mShaderProgram, "pMatrix")));

    //ViewProj, Transform, Color
    //mUniform.insert(std::make_pair<std::string, GLuint>("uViewProj", glGetUniformLocation(mShaderProgram, "uViewProj")));
    mUniform.insert(std::make_pair<std::string, GLuint>("uTransform", glGetUniformLocation(mShaderProgram, "uTransform")));
    mUniform.insert(std::make_pair<std::string, GLuint>("uColor", glGetUniformLocation(mShaderProgram, "uColor")));
    mUniform.insert(std::make_pair<std::string, GLuint>("textureSampler", glGetUniformLocation(mShaderProgram, "textureSampler")));

    uniformsHasBeenSetup = true;
}
