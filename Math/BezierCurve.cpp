#include "Math/BezierCurve.h"

BezierCurve::BezierCurve(Shader& shader)
	:VisualObject(shader)
{
	//mControlPoints.push_back(glm::vec3{ 0.f,0.f,0.f });
	//mControlPoints.push_back(glm::vec3{ 1.f,3.f,0.f });
	//mControlPoints.push_back(glm::vec3{ 4.f,3.f,0.f });
	//mControlPoints.push_back(glm::vec3{ 5.f,0.f,0.f });

	//Alternate Control points
	//mControlPoints.push_back(glm::vec3{ 0.f, 0.f,0.f });
	//mControlPoints.push_back(glm::vec3{ 1.f,-3.f,1.f });
	//mControlPoints.push_back(glm::vec3{ 3.f,-2.f,0.f });
	//mControlPoints.push_back(glm::vec3{ 2.f, 0.f,0.f });

	mControlPoints.push_back(glm::vec3{ -20.f,-20.f,7.f });
	mControlPoints.push_back(glm::vec3{ -20.f,20.f,7.f });
	mControlPoints.push_back(glm::vec3{ 20.f,-20.f,7.f });
	mControlPoints.push_back(glm::vec3{ 20.f,20.f,7.f });

    //glm::vec3 color{ 1.f, 1.f, 1.f };

	mVertices.push_back(Vertex{ mControlPoints[mControlPoints.size() - 1].x, mControlPoints[mControlPoints.size() - 1].y, mControlPoints[mControlPoints.size() - 1].z, 1.f, 1.f, 1.f });

	for (int i = mControlPoints.size() - 1; i > 0; i--)
	{
		mVertices.push_back(Vertex{ mControlPoints[i].x, mControlPoints[i].y, mControlPoints[i].z, 1.f, 1.f, 1.f });
		mVertices.push_back(Vertex{ mControlPoints[i].x, mControlPoints[i].y, mControlPoints[i].z, 1.f, 1.f, 1.f });
	}
	mVertices.push_back(Vertex{ mControlPoints[0].x, mControlPoints[0].y, mControlPoints[0].z, 1.f, 1.f, 1.f });

	//Old way of doing the above
	//mVertices.push_back(Vertex{ 0.f,0.f,0.f,1.f,1.f,1.f });
	//mVertices.push_back(Vertex{ 1.f,-3.f,0.f,1.f,1.f,1.f });
	//mVertices.push_back(Vertex{ 1.f,-3.f,0.f,1.f,1.f,1.f });
	//mVertices.push_back(Vertex{ 3.f,-2.f,0.f,1.f,1.f,1.f });
	//mVertices.push_back(Vertex{ 3.f,-2.f,0.f,1.f,1.f,1.f });
	//mVertices.push_back(Vertex{ 2.f,0.f,0.f,1.f,1.f,1.f });

	construct();
}

BezierCurve::~BezierCurve() 
{

}

void BezierCurve::init()
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

	for (int i = 0; i < mCubes.size(); i++)
	{
		mCubes[i]->init();
	}
}

void BezierCurve::draw()
{

	glBindVertexArray(mVAO);
	glUniformMatrix4fv(mShader.mMatrixUniform, 1, GL_FALSE, glm::value_ptr(mMatrix));
	glDrawArrays(GL_LINE_STRIP, 0, mVertices.size());

	for (int i = 0; i < mCubes.size(); i++)
	{
		mCubes[i]->draw();
	}
}

void BezierCurve::construct()
{
	//Construct the Cubes to visualize the Control Points
	for (int i = 0; i < mControlPoints.size(); i++)
	{
		mCubes.push_back(new Kube(mShader, 0.1f, mControlPoints[i].x, mControlPoints[i].y, mControlPoints[i].z));
	}

	glm::vec3 color{ 1.f, 0.f, 0.f };

	for (auto i = 0.f; i <= 1.0f; i += 0.01f)
	{
		glm::vec3 test = calcBezier(i);
		mVertices.push_back(Vertex{ test.x, test.y, test.z, color.x, color.y, color.z });
	}
}

glm::vec3 BezierCurve::calcBezier(float t)
{
	std::vector<glm::vec3> ctrTmp = mControlPoints;

	for (int k = ctrTmp.size() - 1; k > 0; k--)
	{
		for (int i = 0; i < k; i++)
			ctrTmp[i] = ctrTmp[i] * (1 - t) + ctrTmp[i + 1] * t;
	}
	return ctrTmp[0];
}
