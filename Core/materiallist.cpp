#include "Core/materiallist.h"

//material headers
#include "Materials/materialplain.h"
#include "Materials/materialtexture.h"
#include "Materials/materialphong.h"
#include "Materials/materialcubemap.h" // Used for skybox
#include "Materials/materialbillboard.h"
#include "Materials/materialparticle.h"

#include "stb_image/stb_image.h"



MaterialList::MaterialList()
{
    initializeOpenGLFunctions();
    CompileShaders();
    CompileTextures();
    CompileMaterials();
}

MaterialList::~MaterialList()
{
    for(int i = 0; i < mShaderProgramSize; i++)
        delete mShaderProgram[i].second;
    for(int i = 0; i < mTextureSize; i++)
        delete mTexture[i].second;
    for(int i = 0; i < mMaterialSize; i++)
        delete mMaterial[i].second;
}

void MaterialList::CompileShaders()
{
    const std::string startPath = "../SPIM-Folder/Shaders/";
    std::string shaderPathVert;
    std::string shaderPathFrag;

    //Shader paths that should be compiled.
    // PATHS MUST BE EQUAL TO FILE NAME. Example: path to plainshader.frag/.vert should be written as "plainshader" within the string
    mShaderProgram[0].first = "plainshader";
    mShaderProgram[1].first = "textureshader";
    mShaderProgram[2].first = "phongshader";
    mShaderProgram[3].first = "skyboxshader";
    mShaderProgram[4].first = "plainshaderChad";
    mShaderProgram[5].first = "particleshader";

    for(int i = 0; i < mShaderProgramSize; i++){
        if(!mShaderProgram[i].first.empty()){
            shaderPathVert = startPath;
            shaderPathVert += mShaderProgram[i].first;
            shaderPathVert += ".vert";

            shaderPathFrag = startPath;
            shaderPathFrag += mShaderProgram[i].first;
            shaderPathFrag += ".frag";

            mShaderProgram[i].second = new Shader(shaderPathVert, shaderPathFrag);
        }
        else break;
    }
}

void MaterialList::CompileTextures()
{
    const std::string startPath = "../SPIM-Folder/Assets/Texture/";
    //---------Cubemap-----------
    std::vector<std::string> faces;
    faces.push_back(startPath + "skybox/right.jpg");
    faces.push_back(startPath + "skybox/left.jpg");
    faces.push_back(startPath + "skybox/top.jpg");
    faces.push_back(startPath + "skybox/bottom.jpg");
    faces.push_back(startPath + "skybox/front.jpg");
    faces.push_back(startPath + "skybox/back.jpg");

    cubemapTexture = loadCubemap(faces);

    //-------------Textures-----------
    std::string texturePath;
    int texIndex{0};
    //Default texture (Always slot 0). DO NOT CHANGE
    mTexture[texIndex].first = "default";
    mTexture[texIndex++].second = new Texture();
    //Texture Paths (textures goes here)
    // IMPORTANT: All images must be within the "Assets" folder and be .bmp files.

    //1
    mTexture[texIndex++].first = "linus.bmp";
    //2
    mTexture[texIndex++].first = "EksamenHeightmap.bmp";
    //3
    mTexture[texIndex++].first = "hund.bmp";
    //4
    mTexture[texIndex++].first = "RealGrass.bmp";
    //5
    mTexture[texIndex++].first = "Sand.jpg";
    //6
    mTexture[texIndex++].first = "pew.bmp";
    //7
    mTexture[texIndex++].first = "win.bmp";
    //8
    //mTexture[texIndex++].first = "fire.png";

    //For loop that constructs all the Textures based of the paths given.
    for(int i = 1; i < mTextureSize; i++){
        if(!mTexture[i].first.empty()){
            texturePath = startPath;
            texturePath += mTexture[i].first;

            mTexture[i].second = new Texture(texturePath);
        }
        else break;
    }

    //Bind textures
    auto GL_TEXTURE_UNIT = GL_TEXTURE0;
    for(int i = 0; i < mTextureSize; i++){
        if(!mTexture[i].first.empty())
        {
            glActiveTexture(GL_TEXTURE_UNIT);
            glBindTexture(GL_TEXTURE_2D, mTexture[i].second->id());

            if(GL_TEXTURE_UNIT < GL_TEXTURE31)
                GL_TEXTURE_UNIT++;
            else //Breaks the for loop if there are no more texture unit slots left
                break;
        }
        else break;
    }
}

unsigned int MaterialList::loadCubemap(std::vector<std::string> faces)
{
    /* Notes:
        //GL_TEXTURE_CUBE_MAP_POSITIVE_X	RIGHT
        //GL_TEXTURE_CUBE_MAP_NEGATIVE_X	LEFT
        //GL_TEXTURE_CUBE_MAP_POSITIVE_Y	TOP
        //GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	BOTTOM
        //GL_TEXTURE_CUBE_MAP_POSITIVE_Z	BACK
        //GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	FRONT

        //CONVERTED
        //GL_TEXTURE_CUBE_MAP_POSITIVE_X	RIGHT
        //GL_TEXTURE_CUBE_MAP_NEGATIVE_X	LEFT
        //GL_TEXTURE_CUBE_MAP_POSITIVE_Y	FRONT
        //GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	BACK
        //GL_TEXTURE_CUBE_MAP_POSITIVE_Z	TOP
        //GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	BOTTOM
    */
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            //std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}


void MaterialList::CompileMaterials()
{
    int matIndex{0};
    // Note:
    // Each material keeps track of which shader it's ment to use. Some also require texture Index, or other things
    // Setup:
    //    mMaterial[index].first = "|The name of the material|";
    //    mMaterial[index].second = new |Material class|(mShaderProgram[|mShaderProgram index|].second->getProgram(), |mTexture index (optional)|);
    //    mMaterial[index].second->SetupUniforms();
    //    matIndex++


    //Skybox | Should be first
    mMaterial[matIndex].first = "materialskybox";
    mMaterial[matIndex].second = new MaterialCubemap(mShaderProgram[3].second->getProgram(), cubemapTexture);
    mMaterial[matIndex].second->SetupUniforms();
    matIndex++;

    // Base materials
    mMaterial[matIndex].first = "materialplain";
    mMaterial[matIndex].second = new MaterialPlain(mShaderProgram[0].second->getProgram());
    mMaterial[matIndex].second->SetupUniforms();
    matIndex++;

    mMaterial[matIndex].first = "materialtexture";
    mMaterial[matIndex].second = new MaterialTexture(mShaderProgram[1].second->getProgram(), 5);
    mMaterial[matIndex].second->SetupUniforms();
    matIndex++;

    mMaterial[matIndex].first = "materialphong";
    mMaterial[matIndex].second = new MaterialPhong(mShaderProgram[2].second->getProgram(), 0);
    mMaterial[matIndex].second->SetupUniforms();
    matIndex++;

    // Other materials
    mMaterial[matIndex].first = "materialphongplayer";
    mMaterial[matIndex].second = new MaterialPhong(mShaderProgram[2].second->getProgram(), 5);
    mMaterial[matIndex].second->SetupUniforms();
    matIndex++;

    mMaterial[matIndex].first = "materialbillboard";
    mMaterial[matIndex].second = new MaterialBillboard(mShaderProgram[1].second->getProgram(), 6);
    mMaterial[matIndex].second->SetupUniforms();
    matIndex++;

    mMaterial[matIndex].first = "materialparticle";
    mMaterial[matIndex].second = new MaterialParticle(mShaderProgram[5].second->getProgram(), 6);
    mMaterial[matIndex].second->SetupUniforms();
    matIndex++;
}
