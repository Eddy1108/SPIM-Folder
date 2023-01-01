#include "ParticleSystem.h"
#include "Math/Random.h"
#include "renderwindow.h"
#include "glm/gtx/compatibility.hpp"

ParticleSystem::ParticleSystem(std::string MaterialName) : VisualObject(MaterialName)
{
	mParticlePool.resize(1000);

	mVertices.push_back(Vertex(-0.5f, -0.5f, 0.f,		1.f, 0.f, 0.f,	 0.f, 0.f)); //A
	mVertices.push_back(Vertex(0.5f, -0.5f, 0.f,		0.f, 1.f, 0.f,	 1.f, 0.f)); //C
	mVertices.push_back(Vertex(0.5f, 0.5f, 0.f,			0.f, 0.f, 1.f,	 1.f, 1.f)); //D
	mVertices.push_back(Vertex(-0.5f, 0.5f, 0.f,		1.f, 0.f, 0.f,	 0.f, 1.f)); //A


	uint32_t indices[] = {
	0, 1, 2, 2, 3, 0
	};

	for (size_t i = 0; i < std::size(indices); i++)
	{
		mIndices.push_back(indices[i]);
	}

	mMatrix = glm::mat4(1.0f);
	mPosition = glm::vec3(0.f, 0.f, 3.f);
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::init()
{
	initializeOpenGLFunctions();

	//Vertex Array Object - VAO
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	//Vertex Buffer Object to hold Vertices - VBO
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

	//EBO
	glGenBuffers(1, &mEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

	//1st attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(0));
	glEnableVertexAttribArray(0);

	// 2nd attribute buffer : normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// 3rd attribute buffer : uvs
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

}

void ParticleSystem::draw()
{
	//mMaterial->UpdateUniforms();

	for (auto& particle : mParticlePool)
	{
		if (!particle.Active)
			continue;

		float lifeVal = particle.mLifeRemaining / particle.mLifeTime;

		//Calc Color over time
		glm::vec4 color;
		if (particle.bColorOverTime)
			color = glm::lerp(particle.mColorEnd, particle.mColorBegin, lifeVal);
		else
			color = particle.mColorBegin;

		if (particle.bTransparencyOverTime)
		{
			color.a = glm::lerp(particle.mAlphaEnd, particle.mAlphaBegin, lifeVal);
		}
		

		//Calc Size over time
		float size;
		if (particle.bSizeOverTime)
			size = glm::lerp(particle.mSizeEnd, particle.mSizeBegin, lifeVal);
		else
			size = particle.mSizeBegin;
		
		//Calc transform
		glm::mat4 transform;
		if (particle.bFaceCam)
		{
			transform = glm::translate(
			  glm::mat4(1.0f), { particle.mPosition.x, particle.mPosition.y, particle.mPosition.z })
			* RotateToCamMatrix()
			* glm::scale(glm::mat4(1.0f), { size, size, 1.0f }
			);
		}
		else 
		{
			transform = glm::translate(
			  glm::mat4(1.0f), { particle.mPosition.x, particle.mPosition.y, particle.mPosition.z })
			* glm::rotate(glm::mat4(1.0f), particle.mRotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size, size, 1.0f }
			);
		}
		
		//Render
		mMaterial->UpdateUniforms(&transform, &color);
		glBindVertexArray(mVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


	}
	Update();

	//std::cout << "DRAW CALL DONE" << std::endl;
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

		//Update Rotation
		particle.mRotation += 0.01f * 0.01f;

		//Update Position
		if (particle.bUseGravity)
		{
			particle.mVelocity = particle.mVelocity + glm::vec3{ 0.f,0,-9.81f } * 0.01f;
			particle.mPosition += particle.mVelocity * 0.01f;
		}
		else
			particle.mPosition += particle.mVelocity * 0.01f;


		//Deltatime version, buggy atm
		//particle.mLifeRemaining -= RenderWindow::mDeltaTime;
		//particle.mPosition += particle.mVelocity * (float)RenderWindow::mDeltaTime;
		//particle.mRotation += 0.01f  * RenderWindow::mDeltaTime;
	}
}

void ParticleSystem::Emit(const ParticleProperties& particleProps)
{
	double timeCheck = 1.f / particleProps.SpawnRate;	// 1/5 = 0.2

	timeTaken += RenderWindow::mDeltaTime;
	//std::cout << "Delta: " << timeTaken << std::endl;	//Its so strange

	if (timeTaken < timeCheck)
	{
		return;
	}

	timeTaken -= timeCheck;

	Particle& particle = mParticlePool[mPoolIndex];
	particle.Active = true;
	particle.bFaceCam = particleProps.bFaceCamera;
	particle.bUseGravity = particleProps.bUseGravity;
	particle.bSizeOverTime = particleProps.bSizeOverTime;
	particle.bColorOverTime = particleProps.bColorOverTime;
	particle.bTransparencyOverTime = particleProps.bTransparencyOverTime;

	particle.mPosition = particleProps.Position;
	particle.mRotation = Random::Float() * 2.0f * glm::pi<float>();

	//Velocity
	particle.mVelocity = particleProps.Velocity;
	particle.mVelocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
	particle.mVelocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);
	particle.mVelocity.z += particleProps.VelocityVariation.z * (Random::Float() - 0.5f);

	//Color
	particle.mColorBegin = particleProps.ColorBegin; particle.mColorBegin.a = 1.0f;
	particle.mColorEnd = particleProps.ColorEnd; particle.mColorEnd.a = 1.0f;
	particle.mAlphaBegin = particleProps.ColorBegin.a;
	particle.mAlphaEnd = particleProps.ColorEnd.a;

	particle.mLifeTime = particleProps.LifeTime;
	particle.mLifeRemaining = particleProps.LifeTime;
	particle.mSizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
	particle.mSizeEnd = particleProps.SizeEnd;

	particle.mSpawnRate = particleProps.SpawnRate;

	mPoolIndex = --mPoolIndex % mParticlePool.size();
}
