#pragma once

#include "Core/material.h"

class MaterialParticle : public Material
{
public:
    MaterialParticle(GLuint shaderProgram, int textureIndex);
    //void Init() override;
    void UpdateUniforms() override;
    void UpdateUniforms(glm::mat4* transform, glm::vec4* color) override;
    void SetupUniforms() override;

};

