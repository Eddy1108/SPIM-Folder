#ifndef MATERIALPLAIN_H
#define MATERIALPLAIN_H

#include "Core/material.h"

class MaterialPlain : public Material
{
public:
    MaterialPlain(GLuint shaderProgram);
    //void Init() override;
    void UpdateUniforms(glm::mat4* mMatrix) override;
    void SetupUniforms() override;

};
#endif // MATERIALPLAIN_H
