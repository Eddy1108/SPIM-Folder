#ifndef MATERIALCUBEMAP_H
#define MATERIALCUBEMAP_H

#include "Core/material.h"

class MaterialCubemap : public Material
{
public:
    MaterialCubemap(GLuint shaderProgram, int textureIndex);
    void UpdateUniforms(unsigned int VAO) override;
    void SetupUniforms() override;
};

#endif // MATERIALCUBEMAP_H
