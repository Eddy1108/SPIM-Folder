#pragma once
#include "Components/BaseComponent.h"
#include "Particles/ParticleSystem.h"

class ParticleComponent : public BaseComponent
{
public:
	ParticleComponent(VisualObject* parent, bool IsActivatedFromStart, bool AttachToParent, ParticleProperties* particleProps = nullptr, glm::vec3 PositionOffset = {0.f,0.f,0.f});

	void init() override;
	void update() override;
	void activate() override;	//activate/deactivate particles (toggle)

	void SetParticleProps(ParticleProperties* particleProps) { mParticleProps = particleProps; };

protected:

	bool bAttachToParent{ false };
	glm::vec3 mPositionOffset;

	ParticleSystem* mParticleSystem{ nullptr };
	ParticleProperties* mParticleProps{ nullptr };
};

