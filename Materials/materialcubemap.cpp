#include "materialcubemap.h"
#include "glm/gtx/rotate_vector.hpp"

/*MaterialCubemap::MaterialCubemap(GLuint shaderProgram, int textureIndex[6])
    //: Material(shaderProgram, textureIndex[6])
{

}*/

MaterialCubemap::MaterialCubemap(GLuint shaderProgram, int textureIndex)
    : Material(shaderProgram, textureIndex)
{

}

void MaterialCubemap::UpdateUniforms(unsigned int VAO)
{
    initializeOpenGLFunctions();
    //glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
    glUseProgram(mShaderProgram);
    // ... set view and projection matrix
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureIndex[0]);

    //Rotate skybox
    glm::mat4 rotMatrix = glm::mat4(1.0f);
    rotMatrix = glm::rotate(glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f));

    glUniformMatrix4fv(mUniform.find("rotMatrix")->second, 1, GL_FALSE, glm::value_ptr(rotMatrix)); //i have z as up, so need to rotate skybox 90 degrees
}

void MaterialCubemap::SetupUniforms()
{
    initializeOpenGLFunctions();
    if(uniformsHasBeenSetup) return;

    mUniform.insert(std::make_pair<std::string,GLuint>("vMatrix", glGetUniformLocation( mShaderProgram, "vMatrix")));
    mUniform.insert(std::make_pair<std::string,GLuint>("pMatrix", glGetUniformLocation( mShaderProgram, "pMatrix")));
    mUniform.insert(std::make_pair<std::string,GLuint>("skybox", glGetUniformLocation( mShaderProgram, "skybox")));
    mUniform.insert(std::make_pair<std::string,GLuint>("rotMatrix", glGetUniformLocation( mShaderProgram, "rotMatrix")));

    uniformsHasBeenSetup = true;
}
