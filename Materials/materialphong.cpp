#include "materialphong.h"
#include "renderwindow.h"

MaterialPhong::MaterialPhong(GLuint shaderProgram, int textureIndex)
    : Material(shaderProgram, textureIndex)
{

}

void MaterialPhong::UpdateUniforms(glm::mat4* mMatrix, glm::vec3* cameraPosition, glm::vec3* lightPosition)
{
    initializeOpenGLFunctions();
    glUseProgram(mShaderProgram);

    //Update Uniform variables
    glUniformMatrix4fv( mUniform.find("mMatrix")->second, 1, GL_FALSE,  glm::value_ptr(*mMatrix));
    glUniform1i(mUniform.find("textureSampler")->second, mTextureIndex[0]);
    //glUniform3f(mUniform.find("cameraPosition")->second, cameraPosition->x(), cameraPosition->y(), cameraPosition->z());
    glUniform3f(mUniform.find("lightPosition")->second, lightPosition->x, lightPosition->y, lightPosition->z);
}

void MaterialPhong::SetupUniforms()
{
    initializeOpenGLFunctions();
    if(uniformsHasBeenSetup) return;

    mUniform.insert(std::make_pair<std::string,GLuint>("mMatrix", glGetUniformLocation( mShaderProgram, "mMatrix")));
    mUniform.insert(std::make_pair<std::string,GLuint>("vMatrix", glGetUniformLocation( mShaderProgram, "vMatrix")));
    mUniform.insert(std::make_pair<std::string,GLuint>("pMatrix", glGetUniformLocation( mShaderProgram, "pMatrix")));
    mUniform.insert(std::make_pair<std::string,GLuint>("textureSampler", glGetUniformLocation( mShaderProgram, "textureSampler")));
    mUniform.insert(std::make_pair<std::string,GLuint>("cameraPosition", glGetUniformLocation( mShaderProgram, "cameraPosition")));
    mUniform.insert(std::make_pair<std::string,GLuint>("lightPosition", glGetUniformLocation( mShaderProgram, "lightPosition")));

    uniformsHasBeenSetup = true;
}

void MaterialPhong::UpdateCameraUniform(glm::mat4 *vMatrix, glm::mat4 *pMatrix)
{
    initializeOpenGLFunctions();
    glUseProgram(mShaderProgram);

    glUniformMatrix4fv(mUniform.find("vMatrix")->second, 1, GL_FALSE, glm::value_ptr(*vMatrix));
    glUniformMatrix4fv(mUniform.find("pMatrix")->second, 1, GL_FALSE, glm::value_ptr(*pMatrix));

    //Perhaps should be done differently, to prevent using #include "renderwindow"
    glm::vec3* camPos = &RenderWindow::mCurrentCamera->mPosition;
    glUniform3f(mUniform.find("cameraPosition")->second, camPos->x, camPos->y, camPos->z);
}
