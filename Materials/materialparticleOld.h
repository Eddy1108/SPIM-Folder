#ifndef MATERIALPARTICLE_H
#define MATERIALPARTICLE_H

#include "Core/material.h"

class MaterialParticleOld : public Material
{
public:
    MaterialParticleOld(GLuint shaderProgram, int textureIndex);
    //void Init() override;
    void UpdateUniforms() override;
    void SetupUniforms() override;

};

#endif // MATERIALPARTICLE_H
