#ifndef MATERIALPARTICLE_H
#define MATERIALPARTICLE_H

#include "Core/material.h"

class MaterialParticle : public Material
{
public:
    MaterialParticle(GLuint shaderProgram, int textureIndex);
    //void Init() override;
    void UpdateUniforms() override;
    void SetupUniforms() override;

};

#endif // MATERIALPARTICLE_H
