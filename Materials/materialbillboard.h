#pragma once
//#include "Core/shader.h"
//class BillboardShader : public Shader
//{
//public:
//	BillboardShader(const GLchar* vertexPath, const GLchar* fragmentPath);
//};

#include "Core/material.h"

class MaterialBillboard : public Material
{
public:

    MaterialBillboard(GLuint shaderProgram, int textureIndex);
    //void Init() override;
    void UpdateUniforms(glm::mat4* mMatrix) override;
    void SetupUniforms() override;
};