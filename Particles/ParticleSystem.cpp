#include "ParticleSystem.h"
#include "Math/Random.h"
#include "renderwindow.h"
#include "glm/gtx/compatibility.hpp"

ParticleSystem::ParticleSystem(std::string MaterialName) : VisualObject(MaterialName)
{
	mParticlePool.resize(1000);
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::init()
{
	initializeOpenGLFunctions();

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	uint32_t indices[] = {
		0, 1, 2, 2, 3, 0
	};

	glGenBuffers(1, &mEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void ParticleSystem::draw()
{
	//mMaterial->UpdateUniforms();

	for (auto& particle : mParticlePool)
	{
		if (!particle.Active)
			continue;

		float life = particle.mLifeRemaining / particle.mLifeTime;
		glm::vec4 color = glm::lerp(particle.mColorEnd, particle.mColorBegin, life);
		color.a = color.a * life;

		float size = glm::lerp(particle.mSizeEnd, particle.mSizeBegin, life);

		glm::mat4 transform;
		if (particle.bFaceCam)
		{
			transform = glm::translate(glm::mat4(1.0f), { particle.mPosition.x, particle.mPosition.y, particle.mPosition.z })
			* RotateToCamMatrix()
			* glm::scale(glm::mat4(1.0f), { size, size, 1.0f });
		}
		else 
		{
			transform = glm::translate(glm::mat4(1.0f), { particle.mPosition.x, particle.mPosition.y, particle.mPosition.z })
			* glm::scale(glm::mat4(1.0f), { size, size, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size, size, 1.0f });
		}
		
		//Render
		mMaterial->UpdateUniforms(&transform, &color);
		glBindVertexArray(mVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


	}
	Update();

	//std::cout << "DRAW DONE" << std::endl;
}

glm::mat4 ParticleSystem::RotateToCamMatrix()
{
	glm::mat4 billView = glm::inverse(RenderWindow::mCurrentCamera->mVMatrix);

	glm::vec4 leftVectorX = billView[0];
	glm::vec4 upVectorY = billView[1];
	glm::vec4 forwardVectorZ = billView[2];

	//Keep only rotation
	glm::mat4 matrix = glm::mat4(1.0f);
	matrix[0] = leftVectorX;
	matrix[1] = upVectorY;
	matrix[2] = forwardVectorZ;

	return matrix;

}

void ParticleSystem::Update()
{
	for (auto& particle : mParticlePool)
	{
		if (!particle.Active)
		{
			continue;
		}

		if (particle.mLifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}

		particle.mLifeRemaining -= 0.01f;
		particle.mPosition += particle.mVelocity * 0.01f;
		particle.mRotation += 0.01f * 0.01f;

		//Deltatime version, buggy atm
		//particle.mLifeRemaining -= RenderWindow::mDeltaTime;
		//particle.mPosition += particle.mVelocity * (float)RenderWindow::mDeltaTime;
		//particle.mRotation += 0.01f  * RenderWindow::mDeltaTime;
	}
}

void ParticleSystem::Emit(const ParticleProperties& particleProps)
{
	Particle& particle = mParticlePool[mPoolIndex];
	particle.Active = true;
	particle.bFaceCam = particleProps.bFaceCamera;
	particle.mPosition = particleProps.Position;
	particle.mRotation = Random::Float() * 2.0f * glm::pi<float>();

	//Velocity
	particle.mVelocity = particleProps.Velocity;
	particle.mVelocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
	particle.mVelocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);
	particle.mVelocity.z += particleProps.VelocityVariation.z * (Random::Float() - 0.5f);

	//Color
	particle.mColorBegin = particleProps.ColorBegin;
	particle.mColorEnd = particleProps.ColorEnd;

	particle.mLifeTime = particleProps.LifeTime;
	particle.mLifeRemaining = particleProps.LifeTime;
	particle.mSizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
	particle.mSizeEnd = particleProps.SizeEnd;

	mPoolIndex = --mPoolIndex % mParticlePool.size();
}
