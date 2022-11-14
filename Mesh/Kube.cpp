#include "Kube.h"



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
	LoadAudio();

	//initializeOpenGLFunctions();

	////Vertex Array Object - VAO
	//glGenVertexArrays(1, &mVAO);
	//glBindVertexArray(mVAO);

	////Vertex Buffer Object to hold vertices - VBO
	//glGenBuffers(1, &mVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	//glBufferData(GL_ARRAY_BUFFER,      //what buffer type
	//	mVertices.size() * sizeof(Vertex),			   //how big buffer do we need
	//	mVertices.data(),             //the actual vertices
	//	GL_STATIC_DRAW				 //should the buffer be updated on the GPU
	//);

	//// 1st attribute buffer : vertices
	//glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	//glVertexAttribPointer(
	//	0,                  // attribute. No particular reason for 0, but must match layout(location = 0) in the vertex shader.
	//	3,                  // size / number of elements of data type
	//	GL_FLOAT,           // data type
	//	GL_FALSE,           // normalize data
	//	sizeof(Vertex),  // stride
	//	reinterpret_cast<GLvoid*>(0));          // array buffer offset
	//glEnableVertexAttribArray(0);

	//// 2nd attribute buffer : colors
	//// Same parameter list as above but attribute and offset is adjusted accoringly
	//glVertexAttribPointer(
	//	1,
	//	3,
	//	GL_FLOAT,
	//	GL_FALSE,
	//	sizeof(Vertex),
	//	reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);

	//// 3rd attribute buffer : UV
	////glVertexAttribPointer(
	////	2,
	////	2,
	////	GL_FLOAT,
	////	GL_FALSE,
	////	sizeof(Vertex),
	////	reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)));
	////glEnableVertexAttribArray(2);

	//glBindVertexArray(0);	//release

	////mMatrix.translate(1.f, 1.f, 1.f);
	
	VisualObject::init();

	VisualObject::StartAudio();
	
}

void Kube::draw()
{
	static glm::vec3 moveTest{ 0.f,0.f,0.f };
	moveTest.x += 0.01f;
	moveTest.y += 0.01f;

	move(moveTest.x, moveTest.y, moveTest.z);

	mMatrix = glm::rotate(mMatrix, 0.05f, glm::vec3(0.0f, 0.f, 1.f));

	VisualObject::draw();
}

void Kube::LoadAudio()
{
	AudioVec.push_back(new Audio("../SPIM-Folder/Assets/sounds/WhereMono.wav"));
}
