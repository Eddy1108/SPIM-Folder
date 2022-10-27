#include "XYZ.h"

XYZ::XYZ(Shader& shader)
	:VisualObject(shader)
{
	mVertices.push_back(Vertex{ 0,0,0,1,0,0 });
	mVertices.push_back(Vertex{ 3,0,0,1,0,0 });
	mVertices.push_back(Vertex{ 0,0,0,0,1,0 });
	mVertices.push_back(Vertex{ 0,3,0,0,1,0 });
	mVertices.push_back(Vertex{ 0,0,0,0,0,1 });
	mVertices.push_back(Vertex{ 0,0,3,0,0,1 });
	mMatrix = glm::mat4(1.0f);
}

XYZ::~XYZ() {    }

void XYZ::init() {

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

	glBindVertexArray(0);	//release
}

void XYZ::draw()
{
	if (bDraw)	//When in editor mode, draw XYZ!
	{
		mShader.use();

		glBindVertexArray(mVAO);
		glUniformMatrix4fv(mShader.mMatrixUniform, 1, GL_FALSE, glm::value_ptr(mMatrix));
		glDrawArrays(GL_LINES, 0, mVertices.size());
	}
}

