#include "Graph.h"

Graph::Graph(Shader& shader, bool b)
	:VisualObject(shader)
{
	mGraphs.push_back(new Graph(shader, 1));
	mGraphs.push_back(new Graph(shader, 2));

	bMaster = b;
}

Graph::Graph(Shader& shader, int graph) : VisualObject(shader), graphUse{ graph }
{

	switch (graph)
	{
	case 1:
		points.push_back(QVector2D{ 2.f, 4.f });
		points.push_back(QVector2D{ 4.f, 3.f });
		points.push_back(QVector2D{ 6.f, 6.f });
		points.push_back(QVector2D{ 9.f, 2.f });
		points.push_back(QVector2D{ 11.f, 3.f });
		points.push_back(QVector2D{ 8.f, 6.f });
		points.push_back(QVector2D{ 13.f, 5.f });

		readFile("Graph1.txt");
		break;
	case 2:
		//4.6.7
		points.push_back(QVector2D{ -3.f, -2.f });
		points.push_back(QVector2D{ -1.f, 3.f });
		points.push_back(QVector2D{ 3.f, -1.f });
		points.push_back(QVector2D{ 6.f, 1.f });

		readFile("Graph2.txt");
		break;
	}





	
	//construct();
	constructPoints();
	mMatrix = glm::mat4(1.0f);
}

Graph::~Graph()
{
}

void Graph::init()
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

	if (mGraphs.size() > 0)
	{
		for (int i = 0; i < mGraphs.size(); i++)
		{
			mGraphs[i]->init();
		}
	}

	if (Cubes.size() > 0)
	{
		for (auto i = 0; i < Cubes.size(); i++)
		{
			Cubes[i]->init();
		}
	}

}

void Graph::draw()
{

	if (bMaster)
	{
		mGraphs[graphDraw - 1]->draw();
	}
	else 
	{

		glBindVertexArray(mVAO);
		glUniformMatrix4fv(mShader.mMatrixUniform, 1, GL_FALSE, glm::value_ptr(mMatrix));
		glDrawArrays(GL_LINES, 0, mVertices.size());

		if (Cubes.size() > 0)
		{
			for (auto i = 0; i < Cubes.size(); i++)
			{
				Cubes[i]->draw();
			}
		}
	
	}
}

void Graph::construct()
{
	float xmin = -10.f, xmax = 50.f, h = 1.f;
	for (auto x = xmin; x < xmax; x += h)
	{
		mVertices.push_back(Vertex{ x, mathFunc(x),0, 1, 1, 1 });
		mVertices.push_back(Vertex{ x+h, mathFunc(x+h),0, 1, 1, 1 });
	}

	writeFile("Graph1.txt");
}

void Graph::constructPoints()
{
	if (points.size() > 0)
	{
		for (auto i = 0; i < points.size(); i++)
		{
			Cubes.push_back(new Kube(mShader, 0.1f, points[i].x(), points[i].y(), 0.f));
		}
	}
}

void Graph::readFile(std::string filename)
{
	std::ifstream inn;
	inn.open(filename.c_str());

	if (inn.is_open()) {
		int n;
		Vertex vertex;
		inn >> n;
		mVertices.reserve(n);
		for (int i = 0; i < n; i++) {
			inn >> vertex;
			mVertices.push_back(vertex);
		}
		inn.close();
	}
}

void Graph::writeFile(std::string filename)
{
	std::fstream in;
	in.open(filename.c_str());

	in << mVertices.size() << std::endl;

	for (float i = 0; i < mVertices.size(); i++)
	{
		in << "(" << mVertices[i].m_xyz[0] << ", " << mVertices[i].m_xyz[1] << ", " << mVertices[i].m_xyz[2] << ")";
		in << "(" << mVertices[i].m_normal[0] << ", " << mVertices[i].m_normal[1] << ", " << mVertices[i].m_normal[2] << ")";
		in << "(" << mVertices[i].m_uv[0] << ", " << mVertices[i].m_uv[1] << ")" << std::endl;

	}
}

bool Graph::activate(float f)
{
	if (graphDraw == 1)
	{
		graphDraw = 2;
	}
	else if (graphDraw == 2)
	{
		graphDraw = 1;
	}

	//graphDraw++;

	//if (graphDraw > mGraphs.size())
	//{
	//	graphDraw = 1;
	//}

	return true;
}

float Graph::mathFunc(float x)
{
	return ((-1027.f/155316.f)* pow(x,2) + (16075.f / 155316.f) * x + (98963.f/25886.f));
}

float Graph::mathFunc2(float x)
{
	return ((23.f / 252.f) * pow(x, 3) + (-31.f/63.f) * pow(x,2) + (-55.f/84.f) * x + 41.f/14.f);
}
