#pragma once
#include "Core/VisualObject.h"
#include "glm/glm.hpp"
#include "Core/texture.h"

///Simple Light class that has a small mesh and some light parameters
class Light : public VisualObject
{
public:
    ///Light class with mesh and light parameters
    Light(Shader& shader);
    ~Light();
    virtual void init() override;
    virtual void draw() override;

    GLfloat mAmbientStrenght{ 0.3f };
    glm::vec3 mAmbientColor{ 0.3f, 0.3f, 0.3f };  //Grey

    GLfloat mLightStrenght{ 0.7f };
    glm::vec3 mLightColor{ 0.9f, 0.9f, 0.3f };    //Yellow

    GLfloat mSpecularStrenght{ 0.9f };
    GLint mSpecularExponent{ 4 };

    Texture* mTexture{ nullptr };
};

