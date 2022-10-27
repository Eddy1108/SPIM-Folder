#include "Triangle.h"

Triangle::Triangle(Shader& shader)
	:VisualObject(shader)
{

	mVertices.push_back(Vertex{ 0.f, 0.f, 0.f,   0,0,0, 0,0 }); // A
	mVertices.push_back(Vertex{ 1.f, 0.f, 0.f,   1,0,0 ,1,0 }); // B
	mVertices.push_back(Vertex{ 0.5f, 0.f, 1.f,  0,0,1 ,0,1 }); // C

	mMatrix = glm::mat4(1.0f);
	
}

Triangle::~Triangle()
{
}

void Triangle::init()
{

	initializeOpenGLFunctions();

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
	mTexture = new Texture("../3DProgExam/Assets/tex/hund.bmp");
}

void Triangle::draw()
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mTexture->id());

	glBindVertexArray(mVAO);
	glUniformMatrix4fv(mShader.mMatrixUniform, 1, GL_FALSE, glm::value_ptr(mMatrix));
	glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
	glBindVertexArray(0);
}
