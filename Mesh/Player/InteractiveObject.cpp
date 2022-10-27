#include "InteractiveObject.h"

#include "mesh/Heightmap.h"

InteractiveObject::InteractiveObject(Shader& shader)
	:VisualObject(shader)
{

	float a = 0.5f;
	float b = -0.5f;

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

	bBypass = true;
	move(5, 5, 0);
	bBypass = false;

	mBShape = new AABB();
	//dynamic_cast<AABB*>(mBShape)->mExtent *= 2.f;

}

InteractiveObject::~InteractiveObject()
{
}

void InteractiveObject::init()
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

	glBindVertexArray(0);	//release

	
}

void InteractiveObject::draw()
{
	VisualObject::draw();
}

void InteractiveObject::move(float x, float y, float z)
{
	if (WMove) my += y;

	if (SMove) my -= y;

	if (AMove) mx -= x;

	if (DMove) mx += x;

	if (QMove) mz += z;

	if (EMove) mz -= z;
	
	if (bBypass)
	{
		mx += x;
		my += y;
		mz += z;
	}


	//float temp = ((sin(mx + my)) - mz);

	//mMatrix.translate(x, y, temp);
	//mMatrix.translate(x, y, z);

	/*mz = sin(mx + my);*/

	if (mHeightmap && mHeightmap->IsInside(glm::vec3(mx, my, mz)))
	{
		mz = mHeightmap->getHeight(glm::vec3(mx, my, mz));
	}
	
	mMatrix = glm::mat4(1.0f);	//NEED THIS FOR EACH TIME YOU MOVE IT WITH TRANSLATE
	glm::vec3 pos( mx,my,mz );
	mMatrix = glm::translate(mMatrix, pos);
	mPosition = pos;


	if (mBShape)
	{
		mBShape->mPosition = mPosition;
	}
}
