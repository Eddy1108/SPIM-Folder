#pragma once
#include "Core/VisualObject.h"

struct ParticleProperties {
	glm::vec3 Position;
	glm::vec3 Velocity, VelocityVariation;
	glm::vec4 ColorBegin, ColorEnd;
	float SizeBegin, SizeEnd, SizeVariation;
	float LifeTime{ 1.0f };
	bool bFaceCamera{ false };
	bool bUseGravity{ false };
	bool bSizeOverTime{ false };
	bool bColorOverTime{ false };
	bool bTransparencyOverTime{ false };
};

class ParticleSystem : public VisualObject
{
public:
	ParticleSystem(std::string MaterialName);
	~ParticleSystem();

	void init();
	void draw();

	glm::mat4 RotateToCamMatrix();

	//Updates particles
	void Update();

	//Spawns 1 particle
	void Emit(const ParticleProperties& particleProps);

private:
	struct Particle {
		glm::vec3 mPosition;
		glm::vec3 mVelocity;
		glm::vec4 mColorBegin, mColorEnd;
		float mRotation{ 0.0f };
		float mSizeBegin, mSizeEnd;
		float mLifeTime{ 1.0f };
		float mLifeRemaining = 0.0f;
		float mAlphaBegin, mAlphaEnd;

		bool bFaceCam{ false };
		bool bUseGravity{ false };
		bool bSizeOverTime{ false };
		bool bColorOverTime{ false };
		bool bTransparencyOverTime{ false };

		bool Active{ false };
	};

	std::vector<Particle> mParticlePool;
	uint32_t mPoolIndex{ 999 };
};

