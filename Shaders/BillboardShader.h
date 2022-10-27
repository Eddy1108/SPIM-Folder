#pragma once
#include "Core/shader.h"
class BillboardShader : public Shader
{
public:
	BillboardShader(const GLchar* vertexPath, const GLchar* fragmentPath);
};

