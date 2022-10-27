#include "Disc.h"


Disc::Disc(Shader& shader)
	:VisualObject(shader)
{
	construct();
	mMatrix = glm::mat4(1.0f);
}

Disc::Disc(Shader& shader, std::string filename)
	:VisualObject(shader)
{
}

Disc::~Disc()
{
}

void Disc::init()
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
	glGenBuffers(1, &mIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

	//1st attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(0));
	glEnableVertexAttribArray(0);

	//2nd attribute buffer : colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//enable the matrixUniform
	// mMatrixUniform = glGetUniformLocation(matrixUniform, "matrix");



	glBindVertexArray(0);

	mVelocity = glm::vec3( 0.1f, 0.0f, 0.0f);
}

void Disc::draw() 
{
	initializeOpenGLFunctions();

	glBindVertexArray(mVAO);
	//GL_FALSE for QMatrix4x4
	glUniformMatrix4fv(mShader.mMatrixUniform, 1, GL_FALSE, glm::value_ptr(mMatrix));
	glDrawElements(GL_TRIANGLE_FAN, mVertices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0)); //mVertices.size();
}

void Disc::construct() 
{
	mVertices.push_back(Vertex{ 0,0,0,1,1,1 });
	for (int i = 0; i < 13; i++)
	{
		double angle = 30 * i * M_PI / 180;
		qDebug() << "angle = " << angle;
		float x = cos(angle);
		float y = sin(angle);
		float z = 0.0f;
		float r = i % 2;
		float g = 0.f;
		float b = 0.5f;
		mVertices.push_back(Vertex{ x,y,z,r,g,b });
	}
	for (GLuint i = 0; i < 14; i++) 
	{
		mIndices.push_back(i);
	}
}

//void Disc::move(float dt)
//{
//	float degrees = (180 * dt) / M_PI;
//	mRotation.rotate(degrees, 0, 0, 1);
//	mMatrix = mRotation;
//}

void Disc::move(float dt)
{
	glm::vec3 ds = mVelocity * dt;
	glm::translate(mMatrix, ds);
	mPosition = ds;

	float degrees = (180 * dt) / M_PI;
	glm::rotate(mRotation, degrees, glm::vec3(0.f, 0.f, 1.f));
	

	glm::vec3 normal = glm::vec3{ 0.0f,1.0f, 0.0f };

	glm::vec3 rotation = glm::cross(normal, mVelocity);
	glm::normalize(rotation);
	
	mMatrix = mMatrix * mRotation;
	//Switch mPosition and mRotation for different results
}