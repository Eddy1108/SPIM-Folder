#include "BillboardShader.h"


BillboardShader::BillboardShader(const GLchar* vertexPath, const GLchar* fragmentPath)
	: Shader(vertexPath, fragmentPath)
{
    mMatrixUniform = glGetUniformLocation(getProgram(), "mMatrix");
    mPMatrixUniform = glGetUniformLocation(getProgram(), "pMatrix");
    mVMatrixUniform = glGetUniformLocation(getProgram(), "vMatrix");
    mTextureUniform = glGetUniformLocation(getProgram(), "textureSampler");
}
