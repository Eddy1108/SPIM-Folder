#include "ParticleComponent.h"

ParticleComponent::ParticleComponent(VisualObject* parent, bool IsActivatedFromStart, bool AttachToParent, ParticleProperties* particleProps, glm::vec3 PositionOffset)
	: BaseComponent(parent, IsActivatedFromStart), 
	bAttachToParent{ AttachToParent },
	mParticleProps{ particleProps },
	mPositionOffset{ PositionOffset }
{
	mParticleSystem = new ParticleSystem("materialparticle");
	mParticleSystem->setName(mComponentName);
}

void ParticleComponent::init()
{
	mParticleSystem->init();

	if (!mParticleProps)
	{
		mParticleProps = new ParticleProperties;

		mParticleProps->ColorBegin = { 1, 1, 1, 1.0f };
		mParticleProps->ColorEnd = { 244 / 255.0f, 211 / 255.0f, 35 / 255.0f, 0.0f };
		mParticleProps->SizeBegin = 0.8f, mParticleProps->SizeVariation = 0.3f, mParticleProps->SizeEnd = 0.0f;
		mParticleProps->LifeTime = 0.5f;
		mParticleProps->Velocity = { 0.0f, 0.0f, 0.0f };
		mParticleProps->VelocityVariation = { 8.0f, 8.0f, 8.0f };
		mParticleProps->Position = { 0.0f, 0.0f, 0.0f };
		mParticleProps->SpawnRate = 200;
					  
		mParticleProps->bFaceCamera = false;
		mParticleProps->bUseGravity = true;
		mParticleProps->bSizeOverTime = false;
		mParticleProps->bColorOverTime = false;
		mParticleProps->bTransparencyOverTime = true;
	}
}

void ParticleComponent::update()
{
	if (bIsActive)
	{
		glm::vec3 parentPos = mParent->getPosition3D();
		mParticleProps->Position = parentPos + mPositionOffset;
		mParticleSystem->Emit(*mParticleProps);
		mParticleSystem->draw();
	}
}

void ParticleComponent::activate()
{
	bIsActive = !bIsActive;
}
