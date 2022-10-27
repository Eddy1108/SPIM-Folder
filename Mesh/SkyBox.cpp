#include "SkyBox.h"

#include "glm/gtx/rotate_vector.hpp"
#include <stb_image/stb_image.h>

SkyBox::SkyBox(Shader& shader)
	: VisualObject(shader)
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
	glGenBuffers(1, &mEAB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

	//1st attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(0));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	//GL_TEXTURE_CUBE_MAP_POSITIVE_X	RIGHT
	//GL_TEXTURE_CUBE_MAP_NEGATIVE_X	LEFT
	//GL_TEXTURE_CUBE_MAP_POSITIVE_Y	TOP
	//GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	BOTTOM
	//GL_TEXTURE_CUBE_MAP_POSITIVE_Z	BACK
	//GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	FRONT

	//CONVERTED
	//GL_TEXTURE_CUBE_MAP_POSITIVE_X	RIGHT
	//GL_TEXTURE_CUBE_MAP_NEGATIVE_X	LEFT
	//GL_TEXTURE_CUBE_MAP_POSITIVE_Y	FRONT
	//GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	BACK
	//GL_TEXTURE_CUBE_MAP_POSITIVE_Z	TOP
	//GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	BOTTOM

    CubemapDir.push_back("../SPIM-Folder/assets/tex/skybox/right.jpg");
    CubemapDir.push_back("../SPIM-Folder/assets/tex/skybox/left.jpg");
    CubemapDir.push_back("../SPIM-Folder/assets/tex/skybox/top.jpg");
    CubemapDir.push_back("../SPIM-Folder/assets/tex/skybox/bottom.jpg");
    CubemapDir.push_back("../SPIM-Folder/assets/tex/skybox/front.jpg");
    CubemapDir.push_back("../SPIM-Folder/assets/tex/skybox/back.jpg");

	//CubemapDir.push_back("../3DProgExam/assets/tex/skybox/right.jpg");
	//CubemapDir.push_back("../3DProgExam/assets/tex/skybox/left.jpg");
	//CubemapDir.push_back("../3DProgExam/assets/tex/skybox/front.jpg");
	//CubemapDir.push_back("../3DProgExam/assets/tex/skybox/back.jpg");
	//CubemapDir.push_back("../3DProgExam/assets/tex/skybox/top.jpg");
	//CubemapDir.push_back("../3DProgExam/assets/tex/skybox/bottom.jpg");

	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);


	//Load Cubemap Textures
	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(CubemapDir[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else 
		{
			std::cout << "Image could NOT LOAD: " << CubemapDir[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


}

void SkyBox::draw()
{

	glBindVertexArray(mVAO);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
