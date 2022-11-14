#ifndef MATERIALPHONG_H
#define MATERIALPHONG_H

#include "Core/material.h"

class MaterialPhong : public Material
{
public:
    MaterialPhong(GLuint shaderProgram, int textureIndex);
    //void Init() override;
    void UpdateUniforms(glm::mat4* mMatrix, glm::vec3* cameraPosition, glm::vec3* lightPosition) override;
    void SetupUniforms() override;
    void UpdateCameraUniform(glm::mat4* vMatrix, glm::mat4* pMatrix) override;

};

#endif // MATERIALPHONG_H
