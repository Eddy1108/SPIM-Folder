#include "Particles/ParticleSystem.h"
#include "renderwindow.h"

ParticleSystem::ParticleSystem(std::string materialName, Camera* cam) : VisualObject(materialName), mCam{cam}
{
	initializeOpenGLFunctions();

	g_particule_position_size_data = new GLfloat[MaxParticles * 4];
	g_particule_color_data = new GLubyte[MaxParticles * 4];

	//VBO that holds 4 vertices for the particles
	static const GLfloat VertexData[] = 
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
	};

	//billboard_vertex_buffer
	//GLuint billboardVertexBuffer;
	glGenBuffers(1, &billboardVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboardVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);

	//VBO that holds the position and size of particles
	//GLuint particlePositionBuffer;
	glGenBuffers(1, &particlePositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, particlePositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	//VBO that holds the color of particles
	//GLuint particleColorBuffer;
	glGenBuffers(1, &particleColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, particleColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::init()
{
	initializeOpenGLFunctions();

	glBindBuffer(GL_ARRAY_BUFFER, particlePositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

	glBindBuffer(GL_ARRAY_BUFFER, particleColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);


	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboardVertexBuffer);
	glVertexAttribPointer(
		0, // attribute. No particular reason for 0, but must match the layout in the shader.
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
	);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, particlePositionBuffer);
	glVertexAttribPointer(
		1, // attribute. No particular reason for 1, but must match the layout in the shader.
		4, // size : x + y + z + size => 4
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
	);

	// 3rd attribute buffer : particles' colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, particleColorBuffer);
	glVertexAttribPointer(
		2, // attribute. No particular reason for 1, but must match the layout in the shader.
		4, // size : r + g + b + a => 4
		GL_UNSIGNED_BYTE, // type
		GL_TRUE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0, // stride
		(void*)0 // array buffer offset
	);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void ParticleSystem::draw()
{
	mMaterial->UpdateUniforms();

	int newparticles = (int)(RenderWindow::mDeltaTime * 10000.f);
	if (newparticles > (int)(0.016f * 10000.f) && std::size(ParticleContainer) < MaxParticles)
	{
		newparticles = (int)(0.016f * 10000.f);
	}
	
	for (int i = 0; i < newparticles; i++) {
		int particleIndex = FindUnusedParticle();
		ParticleContainer[particleIndex].life = 5.0f; // This particle will live 5 seconds.
		ParticleContainer[particleIndex].pos = glm::vec3(0, 0, -20.0f);

		float spread = 1.5f;
		glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);


		glm::vec3 randomdir = glm::vec3(
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f
		);

		ParticleContainer[particleIndex].speed = maindir + randomdir * spread;


		//// Very bad way to generate a random color
		ParticleContainer[particleIndex].r = rand() % 256;
		ParticleContainer[particleIndex].g = rand() % 256;
		ParticleContainer[particleIndex].b = rand() % 256;
		ParticleContainer[particleIndex].a = (rand() % 256) / 3;

		ParticleContainer[particleIndex].size = (rand() % 1000) / 2000.0f + 0.1f;

	}


	for (int i = 0; i < MaxParticles; i++)
	{
		Particle& p = ParticleContainer[i];
		if (p.life > 0.0f)
		{
			p.life -= RenderWindow::mDeltaTime;
			if (p.life > 0.0f)
			{
				p.speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float)RenderWindow::mDeltaTime * 0.5f;
				p.pos += p.speed * (float)RenderWindow::mDeltaTime;
				p.cameraDistance = glm::length2(p.pos - RenderWindow::mCurrentCamera->getPos());

				g_particule_position_size_data[4 * ParticlesCount + 0] = p.pos.x;
				g_particule_position_size_data[4 * ParticlesCount + 1] = p.pos.y;
				g_particule_position_size_data[4 * ParticlesCount + 2] = p.pos.z;

				g_particule_position_size_data[4 * ParticlesCount + 3] = p.size;

				g_particule_color_data[4 * ParticlesCount + 0] = p.r;
				g_particule_color_data[4 * ParticlesCount + 1] = p.g;
				g_particule_color_data[4 * ParticlesCount + 2] = p.b;
				g_particule_color_data[4 * ParticlesCount + 3] = p.a;
			}
			else
			{
				p.cameraDistance = -1.0f;
			}
			ParticlesCount++;
		}
	}
	
	SortParticles();

	
	//init();



	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
	glVertexAttribDivisor(2, 1); // color : one per quad -> 1

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);
	
	ParticlesCount = 0;
}

int ParticleSystem::FindUnusedParticle()
{
	for (int i = LastUsedParticle; i < MaxParticles; i++)
	{
		if (ParticleContainer[i].life < 0)
		{
			LastUsedParticle = i;
			return i;
		}
	}
	for (int i = 0; i < LastUsedParticle; i++)
	{
		if (ParticleContainer[i].life < 0)
		{
			LastUsedParticle = i; 
			return i;
		}
	}

	return 0;
}

void ParticleSystem::SortParticles() 
{
	std::sort(&ParticleContainer[0], &ParticleContainer[MaxParticles]);
}
