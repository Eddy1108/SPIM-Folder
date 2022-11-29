#include "Core/VisualObject.h"
#include "Core/Camera.h"
#include "Core/texture.h"

struct Particle {
	glm::vec3 pos, speed;
	unsigned char r, g, b, a; //Color
	float size, angle, weight;
	float life;
	float cameraDistance;

	bool operator<(const Particle& that) const {
		return this->cameraDistance > that.cameraDistance;
	}
};


class ParticleSystemOld : public VisualObject
{
public:

	ParticleSystemOld(std::string materialName, Camera* cam);
	~ParticleSystemOld();

	void init() override;
	void draw() override;

//	void move();

	glm::vec3 mUp = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 mOrientation = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 billView = glm::mat4(1.0f);

private:
	Camera* mCam{ nullptr };

	Texture* mTexture{ nullptr };

	//void RotateToCamMatrix();
	//void RotateToCamDirect();



public:
	int FindUnusedParticle();
	void SortParticles();

	GLuint VertexArrayID;
	GLuint billboardVertexBuffer{0};
	GLuint particlePositionBuffer{0};
	GLuint particleColorBuffer{0};

	const static int MaxParticles{ 100000 };

	Particle ParticleContainer[MaxParticles];

	int ParticlesCount{ 0 };
	int LastUsedParticle{ 0 };

	GLfloat* particlePositionSizeData;
	GLubyte* particleColorData;
};
