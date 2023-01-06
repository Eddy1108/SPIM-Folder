#pragma once
#include "Core/VisualObject.h"

struct ParticleProperties {
	glm::vec3 Position, Velocity, VelocityVariation;
	glm::vec4 ColorBegin, ColorEnd;

	int SpawnRate{ 1 };

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

	//Rotates the particle to face the camera
	glm::mat4 RotateToCamMatrix();

	//Updates particles
	//void Update();

	//Spawns 1 particle
	void Spawn(const ParticleProperties& particleProps);

private:

	double timeTaken{ 0.f };

	struct Particle {
		glm::vec3 mPosition, mVelocity;
		glm::vec4 mColorBegin, mColorEnd;

		int mSpawnRate{ 1 };
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

