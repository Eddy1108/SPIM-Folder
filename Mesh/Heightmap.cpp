#include "Heightmap.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include "Math/Barycentric.h"
#include "renderwindow.h"

Heightmap::Heightmap(Shader& shader)
	:VisualObject(shader)
{
}

Heightmap::~Heightmap()
{
}

void Heightmap::init()
{

	initializeOpenGLFunctions();

	//Grass texture from here: https://www.pinterest.com/pin/texture-png-seamless-tileable-grass--596867756834423269/
	mTexture = new Texture("../3DProgExam/Assets/tex/RealGrass.bmp");
	mHeightmap = new Texture("../3DProgExam/Assets/tex/EksamenHeightmap.bmp");

	constructMap();

	mMatrix = glm::mat4(1.0f);

	//Vertex Array Object - VAO
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	//Vertex Buffer Object to hold vertices - VBO
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	glBufferData(GL_ARRAY_BUFFER,      //what buffer type
		mVertices.size() * sizeof(Vertex),			   //how big buffer do we need
		mVertices.data(),             //the actual vertices
		GL_STATIC_DRAW				 //should the buffer be updated on the GPU
	);

	//EBO
	glGenBuffers(1, &mEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);
	
	
	// 1st attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match layout(location = 0) in the vertex shader.
		3,                  // size / number of elements of data type
		GL_FLOAT,           // data type
		GL_FALSE,           // normalize data
		sizeof(Vertex),  // stride
		reinterpret_cast<GLvoid*>(0));          // array buffer offset
	glEnableVertexAttribArray(0);

	// 2nd attribute buffer : colors
	// Same parameter list as above but attribute and offset is adjusted accoringly
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// 3rd attribute buffer : UV
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);



	glBindVertexArray(0);	//release
}

void Heightmap::draw()
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mTexture->id());

	glBindVertexArray(mVAO);

	glUniformMatrix4fv(mShader.mMatrixUniform, 1, GL_FALSE, glm::value_ptr(mMatrix));
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
}

void Heightmap::constructMap()
{

	float mSize = .3f;
	int width, height, nrChannels;

	auto imageData = stbi_load("../3DProgExam/Assets/tex/EksamenHeightmap.bmp", &width, &height, &nrChannels, 1);

	if (imageData)
	{
		mWidth = width;
		mHeight = height;
		mDetail = 1;
		mScale = 0.2f;
		offsetX = 10.f;
		offsetY = 10.f;

		int xMin = 0, xMax = mWidth, yMin = 0, yMax = mHeight;

		//Construct surface
		for (int y = yMin; y < yMax; y += mDetail)
		{
			for (int x = xMin; x < xMax; x += mDetail)
			{
				float u{ (float)x / (float)xMax };
				float v{ (float)y / (float)yMax };

				float z = imageData[x + (y * yMax)] * mSize; // + offset?
				mVertices.push_back(Vertex{ ((float)x - width / 2) * mScale, ((float)y - mHeight/2) * mScale,((float)z) * mScale, 1,1,1, u,v });
			}
		}

		//move(-mVertices[0].m_xyz[0] - width / 2, -mVertices[0].m_xyz[1] - height / 2, -mVertices[0].m_xyz[2]);
		//move(-mVertices[0].m_xyz[0], -mVertices[0].m_xyz[1], -mVertices[0].m_xyz[2]);

		xMax /= mDetail;
		yMax /= mDetail;

		mWidth = xMax;
		mHeight = yMax;

		//Make indices
		for (int y = yMin; y < yMax - 1; y += 1)
		{
			for (int x = xMin; x < xMax - 1; x += 1)
			{
				mIndices.push_back(x + (yMax * y));			//A
				mIndices.push_back(((y + 1) * yMax) + x + 1);	//B
				mIndices.push_back(x + 1 + (yMax * y));		//C

				mIndices.push_back(x + (y * yMax));			//A
				mIndices.push_back(((y + 1) * yMax) + x);		//B
				mIndices.push_back(((y + 1) * yMax) + x + 1); //C

			}
		}

		//Set up UV
		//for (int y = 0; y < height - 2; y += 2)
		//{
		//	for (int x = 0; x < width - 2; x += 2)
		//	{
		//		mVertices[x + (height * y)].m_uv[0] = 1; mVertices[(x * width) + y].m_uv[1] = 0;

		//		mVertices[((y + 1) * height) + x + 1].m_uv[0] = 0; mVertices[((y + 1) * height) + x + 1].m_uv[1] = 1;

		//		mVertices[x + 1 + (height * y)].m_uv[0] = 0; mVertices[x + 1 + (height * y)].m_uv[1] = 0;

		//		mVertices[((y + 1) * height) + x].m_uv[0] = 1; mVertices[((y + 1) * height) + x].m_uv[0] = 1;

		//	}
		//}

		//calculate normals
		for (int y = 1; y < height - 1; y += 1)
		{
			for (int x = 1; x < width - 1; x += 1)
			{

				glm::vec3 a(mVertices[(y * height) + x].getXYZ());
				glm::vec3 b(mVertices[(y * height) + x + 1].getXYZ());
				glm::vec3 c(mVertices[((y + 1) * height) + x + 1].getXYZ());
				glm::vec3 d(mVertices[((y + 1) * height) + x].getXYZ());
				glm::vec3 e(mVertices[(y * height) + x - 1].getXYZ());
				glm::vec3 f(mVertices[((y - 1) * height) + x - 1].getXYZ());
				glm::vec3 g(mVertices[((y - 1) * height) + x].getXYZ());

				auto n0 = glm::cross(b - a, c - a);
				auto n1 = glm::cross(c - a, d - a);
				auto n2 = glm::cross(d - a, e - a);
				auto n3 = glm::cross(e - a, f - a);
				auto n4 = glm::cross(f - a, g - a);
				auto n5 = glm::cross(g - a, b - a);

				glm::vec3 normal = glm::normalize(n0 + n1 + n2 + n3 + n4 + n5);
				//glm::vec3 normal(0, 0, 1);

				mVertices[(y * height) + x].m_normal[0] = normal.x;
				mVertices[(y * height) + x].m_normal[1] = normal.y;
				mVertices[(y * height) + x].m_normal[2] = normal.z;
			}
		}

		stbi_image_free(imageData);
	}
}

bool Heightmap::IsInside(glm::vec3 pos)
{
	if (pos.x > -mWidth / 2 * mScale * mDetail && pos.x < mWidth / 2 * mScale * mDetail - 1 && pos.y > -mHeight / 2 * mScale * mDetail && pos.y < mHeight / 2 * mScale * mDetail - 1)
	{
		return true;
	}

	return false;
}

float Heightmap::getHeight(glm::vec3 position)
{

	if (IsInside(position))
	{
		int x = position.x / mScale / mDetail + mWidth / 2;
		int y = position.y / mScale / mDetail + mWidth / 2;

		auto p1 = x + y * mHeight;
		auto p2 = (x + 1) + (y * mHeight);
		auto p3 = (x + 1) + ((y + 1) * mHeight);
		auto p4 = x + ((y + 1) * mHeight);

		if (p3 > mVertices.size())
			return 1.f;

		glm::vec2 a(mVertices[p1].getXYZ().x, mVertices[p1].getXYZ().y);
		glm::vec2 b(mVertices[p2].getXYZ().x, mVertices[p2].getXYZ().y);
		glm::vec2 c(mVertices[p3].getXYZ().x, mVertices[p3].getXYZ().y);
		glm::vec2 d(mVertices[p4].getXYZ().x, mVertices[p4].getXYZ().y);

		glm::vec3 baryCoords = Barycentric::calcBarycentric(glm::vec2(position.x, position.y), a, b, c);

		//Find triangle to use

		if (Barycentric::barycentricIsInside(baryCoords))
		{
			auto p1h = mVertices[p1].getXYZ().z * baryCoords.x;
			auto p2h = mVertices[p2].getXYZ().z * baryCoords.y;
			auto p3h = mVertices[p3].getXYZ().z * baryCoords.z;

			RenderWindow::triangle[0] = mVertices[p1].getXYZ();
			RenderWindow::triangle[1] = mVertices[p2].getXYZ();
			RenderWindow::triangle[2] = mVertices[p3].getXYZ();

			auto height = p1h + p2h + p3h;

			return height;
		}
		else
		{
			baryCoords = Barycentric::calcBarycentric(glm::vec2(position.x, position.y), a, c, d);

			auto p1h = mVertices[p1].getXYZ().z * baryCoords.x;
			auto p2h = mVertices[p3].getXYZ().z * baryCoords.y;
			auto p3h = mVertices[p4].getXYZ().z * baryCoords.z;

			RenderWindow::triangle[0] = mVertices[p1].getXYZ();
			RenderWindow::triangle[1] = mVertices[p3].getXYZ();
			RenderWindow::triangle[2] = mVertices[p4].getXYZ();

			auto height = p1h + p2h + p3h;

			return height;

		}
	}

}
