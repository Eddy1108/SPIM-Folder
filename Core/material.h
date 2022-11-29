#ifndef MATERIAL_H
#define MATERIAL_H

#include "shader.h"
#include "texture.h"

#include <QMatrix4x4>
#include <unordered_map>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"


//class Shader;

class Material : public QOpenGLFunctions_4_1_Core
{
public:
    Material(GLuint shaderProgram);
    Material(GLuint shaderProgram, int textureIndex);

    virtual void Init();
    virtual void UpdateUniforms() { ; };
    virtual void UpdateUniforms(glm::mat4* mMatrix){;};
    virtual void UpdateUniforms(unsigned int VAO){;};
    virtual void UpdateUniforms(glm::mat4* mMatrix, glm::vec4* v1) { ; };
    virtual void UpdateUniforms(glm::mat4* mMatrix, glm::vec3* v1, glm::vec3* v2){;};
    virtual void UpdateCameraUniform(glm::mat4* vMatrix, glm::mat4* pMatrix);
    virtual void SetupUniforms()=0;
    GLuint GetUniform(std::string uniformName);

    GLuint mShaderProgram;
    std::vector<int> mTextureIndex;
    //Texture* mTexture{nullptr};

    std::unordered_map<std::string, GLuint> mUniform;
protected:
    bool uniformsHasBeenSetup {false};
};

#endif // MATERIAL_H
