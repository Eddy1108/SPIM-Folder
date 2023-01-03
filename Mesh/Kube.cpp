#include "Kube.h"

#include "Scenes/Scene0.h"

Kube::Kube(std::string materialName)
    :VisualObject(materialName)
{
	//float min = 0.5f, max = -0.5;

	float a = 0.5f;
	float b = -0.5f;

	//front
	mVertices.push_back(Vertex{ b, b, b,  0,-1,0, 0,0}); // A
	mVertices.push_back(Vertex{ a, b, b,  0,-1,0 ,1,0}); // B
	mVertices.push_back(Vertex{ b, a, b,  0,-1,0 ,0,1}); // C

	mVertices.push_back(Vertex{ b, a, b,  0,-1,0 }); // C
	mVertices.push_back(Vertex{ a, b, b,  0,-1,0 }); // B
	mVertices.push_back(Vertex{ a, a, b,  0,-1,0 }); // D

	//right
	mVertices.push_back(Vertex{ a, b, b,  1,0,0 }); // B
	mVertices.push_back(Vertex{ a, a, b,  1,0,0 }); // D
	mVertices.push_back(Vertex{ a, b, a,  1,0,0 }); // H

	mVertices.push_back(Vertex{ a, b, a,  1,0,0 }); // H
	mVertices.push_back(Vertex{ a, a, b,  1,0,0 }); // D
	mVertices.push_back(Vertex{ a, a, a,  1,0,0 }); // F

	//back
	mVertices.push_back(Vertex{ b, b, a,  0,1,0 }); // G
	mVertices.push_back(Vertex{ a, b, a,  0,1,0 }); //H
	mVertices.push_back(Vertex{ b, a, a,  0,1,0 }); // E

	mVertices.push_back(Vertex{ b, a, a,  0,1,0 }); // E
	mVertices.push_back(Vertex{ a, b, a,  0,1,0 }); // H
	mVertices.push_back(Vertex{ a, a, a,  0,1,0 }); // F

	//left
	mVertices.push_back(Vertex{ b, b, b,  -1,0,0 }); //A
	mVertices.push_back(Vertex{ b, a, b,  -1,0,0 }); //C
	mVertices.push_back(Vertex{ b, b, a,  -1,0,0 }); //G

	mVertices.push_back(Vertex{ b, b, a,  -1,0,0 }); //G
	mVertices.push_back(Vertex{ b, a, b,  -1,0,0 }); //C
	mVertices.push_back(Vertex{ b, a, a,  -1,0,0 }); //E


	//top
	mVertices.push_back(Vertex{ b, a, b,  0,0,1 }); //C
	mVertices.push_back(Vertex{ a, a, b,  0,0,1 }); //D
	mVertices.push_back(Vertex{ b, a, a,  0,0,1 }); //E

	mVertices.push_back(Vertex{ b, a, a,  0,0,1 }); //E
	mVertices.push_back(Vertex{ a, a, b,  0,0,1 }); //D
	mVertices.push_back(Vertex{ a, a, a,  0,0,1 }); //F

	//bottom
	mVertices.push_back(Vertex{ b, b, b,  0,0,-1 }); // A
	mVertices.push_back(Vertex{ a, b, b,  0,0,-1 }); // B
	mVertices.push_back(Vertex{ b, b, a,  0,0,-1 }); // G

	mVertices.push_back(Vertex{ b, b, a,  0,0,-1 }); // G
	mVertices.push_back(Vertex{ a, b, b,  0,0,-1 }); // B
	mVertices.push_back(Vertex{ a, b, a,  0,0,-1 }); // H

	mMatrix = glm::mat4(1.0f);

	move(1.f, 1.f, 0.f);
}

Kube::Kube(std::string materialName, float size, float x, float y, float z)
    :VisualObject(materialName)
{
	if (size < 0.01f)
	{
		size = 0.01f;
	}
	float a = size;
	float b = -size;

	//front
	mVertices.push_back(Vertex{ b, b, b,  0,0,0 }); // A
	mVertices.push_back(Vertex{ a, b, b,  1,0,0 }); // B
	mVertices.push_back(Vertex{ b, a, b,  0,0,1 }); // C

	mVertices.push_back(Vertex{ b, a, b,  0,0,1 }); // C
	mVertices.push_back(Vertex{ a, b, b,  1,0,0 }); // B
	mVertices.push_back(Vertex{ a, a, b,  1,0,1 }); // D

	//right
	mVertices.push_back(Vertex{ a, b, b,  1,0,0 }); // B
	mVertices.push_back(Vertex{ a, a, b,  1,0,1 }); // D
	mVertices.push_back(Vertex{ a, b, a,  0,1,1 }); // H

	mVertices.push_back(Vertex{ a, b, a,  0,1,1 }); // H
	mVertices.push_back(Vertex{ a, a, b,  1,0,1 }); // D
	mVertices.push_back(Vertex{ a, a, a,  1,1,1 }); // F

	//back
	mVertices.push_back(Vertex{ b, b, a,  1,1,0 }); // G
	mVertices.push_back(Vertex{ a, b, a,  0,1,1 }); //H
	mVertices.push_back(Vertex{ b, a, a,  0,1,0 }); // E

	mVertices.push_back(Vertex{ b, a, a,  0,1,0 }); // E
	mVertices.push_back(Vertex{ a, b, a,  0,1,1 }); // H
	mVertices.push_back(Vertex{ a, a, a,  1,1,1 }); // F

	//left
	mVertices.push_back(Vertex{ b, b, b,  0,0,0 }); //A
	mVertices.push_back(Vertex{ b, a, b,  0,0,1 }); //C
	mVertices.push_back(Vertex{ b, b, a,  1,1,0 }); //G

	mVertices.push_back(Vertex{ b, b, a,  1,1,0 }); //G
	mVertices.push_back(Vertex{ b, a, b,  0,0,1 }); //C
	mVertices.push_back(Vertex{ b, a, a,  0,1,0 }); //E


	//top
	mVertices.push_back(Vertex{ b, a, b,  0,0,1 }); //C
	mVertices.push_back(Vertex{ a, a, b,  1,0,1 }); //D
	mVertices.push_back(Vertex{ b, a, a,  0,1,0 }); //E

	mVertices.push_back(Vertex{ b, a, a,  0,1,0 }); //E
	mVertices.push_back(Vertex{ a, a, b,  1,0,1 }); //D
	mVertices.push_back(Vertex{ a, a, a,  1,1,1 }); //F

	//bottom
	mVertices.push_back(Vertex{ b, b, b,  0,0,0 }); // A
	mVertices.push_back(Vertex{ a, b, b,  1,0,0 }); // B
	mVertices.push_back(Vertex{ b, b, a,  1,1,0 }); // G

	mVertices.push_back(Vertex{ b, b, a,  1,1,0 }); // G
	mVertices.push_back(Vertex{ a, b, b,  1,0,0 }); // B
	mVertices.push_back(Vertex{ a, b, a,  0,1,1 }); // H

	mMatrix = glm::mat4(1.0f);

	move(x, y, z);
}

Kube::~Kube()
{
}

void Kube::init() 
{
	VisualObject::init();


	BaseComponent* temp = new AudioComponent(this, "WhereMono.wav", true);
	temp->setName("AudioComponent");
	mComponents.insert(std::pair<std::string, BaseComponent*>{(temp)->mComponentName, temp});

	temp = new MeshComponent(this, "crew.obj", true, glm::vec3{ 0.f,0.f,1.f }, true);
	temp->setName("MeshCoponent");
	mComponents.insert(std::pair<std::string, BaseComponent*>{(temp)->mComponentName, temp});

	temp = new ParticleComponent(this, true, true, nullptr, glm::vec3{ 0.f,0.f,-1.f });
	temp->setName("ParticleComponent");
	mComponents.insert(std::pair<std::string, BaseComponent*>{(temp)->mComponentName, temp});
	
	VisualObject::initComponents();
	
}

void Kube::draw()
{
	//Components
	VisualObject::updateComponents();


	static glm::vec3 moveTest{ 0.f,0.f,0.f };
	moveTest.x += 0.01f;
	moveTest.y += 0.01f;

	move(moveTest.x, moveTest.y, moveTest.z);

	mMatrix = glm::rotate(mMatrix, 0.05f, glm::vec3(0.0f, 0.f, 1.f));

	VisualObject::draw();
}
