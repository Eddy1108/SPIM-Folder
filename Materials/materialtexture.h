#ifndef MATERIALTEXTURE_H
#define MATERIALTEXTURE_H

#include "Core/material.h"

class MaterialTexture : public Material
{
public:
    MaterialTexture(GLuint shaderProgram, int textureIndex);
    //void Init() override;
    void UpdateUniforms(glm::mat4* mMatrix) override;
    void SetupUniforms() override;

};

#endif // MATERIALTEXTURE_H
