#include "MeshComponent.h"

MeshComponent::MeshComponent(VisualObject* parent, std::string objName, bool ActivateFromStart, glm::vec3 OffsetFromParent, bool AttachToParent) : BaseComponent(parent, ActivateFromStart)
{
	mMeshPath += objName;
	mPositionOffset = OffsetFromParent;
	bAttachToParent = AttachToParent;
	mObjLoader = new ObjLoader(mParent->mMaterialName, mMeshPath);
}

void MeshComponent::init()
{
	if (bAttachToParent)
	{
		mObjLoader->mMatrix = mParent->mMatrix;	//We copy the matrix to make sure rotation and scale is also copied over.
		mObjLoader->move(mObjLoader->mMatrix[3].x + mPositionOffset.x, mObjLoader->mMatrix[3].y + mPositionOffset.y, mObjLoader->mMatrix[3].z + mPositionOffset.z);
	}
	mObjLoader->init();
}

void MeshComponent::update()
{
	if (bAttachToParent)
	{
		mObjLoader->mMatrix = mParent->mMatrix;
		mObjLoader->move(mObjLoader->mMatrix[3].x + mPositionOffset.x, mObjLoader->mMatrix[3].y + mPositionOffset.y, mObjLoader->mMatrix[3].z + mPositionOffset.z);
	}
	mObjLoader->draw();
}

void MeshComponent::activate()
{
}
