#pragma once
#include <QOpenGLFunctions_4_1_Core>
#include <QWindow>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"	//This has glm::lookAt()
#include "glm/gtc/type_ptr.hpp"

#include "mesh/Player/InteractiveObject.h"
#include "Mesh/Player/Player.h"
#include "Core/XYZ.h"
#include "mesh/TriangleSurface.h"
#include "Core/Camera.h"
#include "SceneSwitcher.h"
#include "Core/Shader.h"
#include "Mesh/SkyBox.h"
#include "Light/Light.h"
#include "Light/Sun.h"

#include <unordered_map>
#include "Core/quadtree.h"


class Scene : protected QOpenGLFunctions_4_1_Core
{
public:
	Scene(std::unordered_map<std::string, Shader*> shaders);
	~Scene();

	virtual void init();
	virtual void draw();
	virtual void drawCollision();
	virtual void checkCollision();

	void DrawAABB(glm::vec3 position, glm::vec3 extent);
	void PauseObjects();

	//Plain Objects
	std::vector<VisualObject*> mObjects;
	std::unordered_map<std::string, VisualObject*> mMap;

	//Textured Objects
	std::vector<VisualObject*> mObjects2;
	std::unordered_map<std::string, VisualObject*> mMap2;

	//Phong affected Objects
	std::vector<VisualObject*> mObjects3;
	std::unordered_map<std::string, VisualObject*> mMap3;

	QuadTre* mQuadTre{nullptr};

	Camera* mCamera{nullptr};

	SkyBox* mSkybox{ nullptr };
	Light* mLight{ nullptr };

	//Shader Storage
	std::unordered_map<std::string, Shader*> mShaderPrograms;

	bool bPlayMode{ false };
	
};

