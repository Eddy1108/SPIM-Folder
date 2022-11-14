#include "SkyBox.h"

#include "glm/gtx/rotate_vector.hpp"

SkyBox::SkyBox(std::string materialName)
    : VisualObject(materialName)
{
	mVertices.push_back(Vertex{ -1.0f, -1.0f, 1.0f, 0.f, 0.f, 0.f });
	mVertices.push_back(Vertex{ 1.0f, -1.0f, 1.0f, 0.f, 0.f, 0.f });
	mVertices.push_back(Vertex{ -1.0f, -1.0f, -1.0f, 0.f, 0.f, 0.f });
	mVertices.push_back(Vertex{ -1.0f, -1.0f, -1.0f, 0.f, 0.f, 0.f });
	mVertices.push_back(Vertex{ -1.0f, 1.0f, 1.0f, 0.f, 0.f, 0.f });
	mVertices.push_back(Vertex{ 1.0f, 1.0f, 1.0f, 0.f, 0.f, 0.f });
	mVertices.push_back(Vertex{ 1.0f, 1.0f, -1.0f, 0.f, 0.f, 0.f });
	mVertices.push_back(Vertex{ -1.0f, 1.0f, -1.0f, 0.f, 0.f, 0.f });

	GLint arr[] =
	{
		6, 2, 1,
		1, 5, 6,

		7, 4, 0,
		0, 3, 7,

		6, 5, 4,
		4, 7, 6,

		2, 3, 0,
		0, 1, 2,

		5, 1, 0,
		0, 4, 5,

		6, 7, 3,
		3, 2, 6
	};

	for (int i = 0; i < std::size(arr); i++)
	{
		mIndices.push_back(arr[i]);
	}
}

void SkyBox::init()
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
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void SkyBox::draw()
{
    mMaterial->UpdateUniforms(mVAO);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glBindVertexArray(0);
}
