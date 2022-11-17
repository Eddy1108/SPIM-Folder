#ifndef MATERIALLIST_H
#define MATERIALLIST_H

#include "shader.h"
#include "material.h"
#include "texture.h"
#include <string>

class MaterialList : public QOpenGLFunctions_4_1_Core
{
public:
    MaterialList();
    ~MaterialList();

    // Try not to have these two variables set to anything above 32, due to lack of slots. Future alternative method might be needed.
    const static int mShaderProgramSize{6};
    const static int mTextureSize{10};
    //
    const static int mMaterialSize{10};

    std::pair<std::string, Shader*> mShaderProgram[mShaderProgramSize];
    std::pair<std::string, Texture*> mTexture[mTextureSize];

    std::pair<std::string, Material*> mMaterial[mMaterialSize];

protected:
    void CompileShaders();
    void CompileTextures();
    void CompileMaterials();

private:
    unsigned int loadCubemap(std::vector<std::string> faces);
    unsigned int cubemapTexture;
};

#endif // MATERIALLIST_H
