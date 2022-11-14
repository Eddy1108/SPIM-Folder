#pragma once
#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "texture.h"
#include "vertex.h"
#include "shader.h"
#include "Core/CollisionShapes.h"
#include "Core/Audio.h"
#include "material.h"

class RenderWindow;
class MaterialList;

class VisualObject : public QOpenGLFunctions_4_1_Core {
public:

	VisualObject() = delete;
    VisualObject(std::string materialName);
	~VisualObject();
	virtual void init();
	virtual void initTexture();
	virtual void draw();

    void setMaterial(std::string materialName);
    //void setMaterial(MaterialList* materialList);
    //void setMaterial(Material &material)

	virtual void move(float x, float y, float z);
	virtual void move(float dt) { ; }
	virtual bool activate(float f = 0) { return false; };

	glm::vec2 getPosition2D();
	glm::vec3 getPosition3D();

	void setName(std::string name);
	std::string getName() const;

	glm::mat4 mMatrix{ 1 };
    //Shader& mShader;
    std::string mMaterialName;
    float mx{0.f}, my{0.f}, mz{0.f}; // position

	CollisionShape* mBShape{ nullptr };

	glm::vec3 mCameraOffset{ 1.f };
	glm::vec3 mPosition;

	std::vector<Vertex> mVertices;
	std::vector<GLint> mIndices;

	bool bPlay{ true };



protected:
    Material* mMaterial{nullptr};

	GLuint mVAO{ 0 };
	GLuint mVBO{ 0 };
    GLuint mEBO{ 0 };

	glm::mat4 mRotation{ 1 };
	glm::mat4 mScale{ 1 };
	glm::vec3 mVelocity;

	std::string mName;

	std::vector<Audio*> AudioVec;

	virtual void LoadAudio() { ; }
	void StartAudio();
	void UpdateAudio();
};
