#include "PhongShader.h"

PhongShader::PhongShader(const GLchar* vertexPath, const GLchar* fragmentPath)
	: Shader(vertexPath, fragmentPath)
{
    mMatrixUniform = glGetUniformLocation(getProgram(), "mMatrix");
    mVMatrixUniform = glGetUniformLocation(getProgram(), "vMatrix");
    mPMatrixUniform = glGetUniformLocation(getProgram(), "pMatrix");

    mLightColorUniform = glGetUniformLocation(getProgram(), "lightColor");
    mObjectColorUniform = glGetUniformLocation(getProgram(), "objectColor");
    mAmbientLightStrengthUniform = glGetUniformLocation(getProgram(), "ambientStrengt");
    mLightPositionUniform = glGetUniformLocation(getProgram(), "lightPosition");
    mSpecularStrengthUniform = glGetUniformLocation(getProgram(), "specularStrength");
    mSpecularExponentUniform = glGetUniformLocation(getProgram(), "specularExponent");
    mLightPowerUniform = glGetUniformLocation(getProgram(), "lightPower");
    mCameraPositionUniform = glGetUniformLocation(getProgram(), "cameraPosition");
    mTextureUniform = glGetUniformLocation(getProgram(), "textureSampler");
}
