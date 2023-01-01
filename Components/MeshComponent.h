#pragma once
#include "Components/BaseComponent.h"
#include "Mesh/ObjLoader.h"

class MeshComponent : public BaseComponent
{
public:
	MeshComponent(VisualObject* parent, std::string objName, bool ActivateFromStart, glm::vec3 OffsetFromParent = glm::vec3{ 0,0,0 }, bool bAttachToParent = true);

	void init() override;
	void update() override;
	void activate() override;

protected:

	bool bAttachToParent{ false };
	glm::vec3 mPositionOffset;

	ObjLoader* mObjLoader{ nullptr };

	std::string mMeshPath = "../SPIM-Folder/Assets/models/";
};

