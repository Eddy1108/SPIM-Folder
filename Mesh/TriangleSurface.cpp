#include "TriangleSurface.h"

TriangleSurface::TriangleSurface(Shader& shader)
	:VisualObject(shader)
{
	/*                x    y     z   r g b*/
	Vertex v0{ -20.0,-20.0,-1.0, 0,0,1, 0,0 };    mVertices.push_back(v0);
	Vertex v1{ -20.0, 20.0,-1.0, 0,0,1, 0,1 };    mVertices.push_back(v1);
	Vertex v2{ 20.0,20.0,-1.0, 0,0,1 , 1,1};    mVertices.push_back(v2);

	Vertex v3{ -20.0,-20.0,-1.0, 0,0,1 ,0,0};    mVertices.push_back(v3);
	Vertex v4{ 20.0,-20.0,-1.0, 0,0,1 ,1,0};    mVertices.push_back(v4);
	Vertex v5{ 20.0,20.0,-1.0, 0,0,1 ,1,1};    mVertices.push_back(v5);

	//construct();

	mMatrix = glm::mat4(1.0f);
}

TriangleSurface::TriangleSurface(std::string filnavn, Shader& shader)
	: VisualObject(shader)
{
	mMatrix = glm::mat4(1.0f);
	readFile(filnavn);
	
}

TriangleSurface::~TriangleSurface()
{
}

void TriangleSurface::readFile(std::string filnavn) {
	std::ifstream inn;
	inn.open(filnavn.c_str());

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
	else {
		std::cout << "\n\nERROR: FILE NOT FOUND\n" << std::endl;
	}
}

void TriangleSurface::init() {

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

	mTexture = new Texture("../3DProgExam/Assets/tex/linus.bmp");
}

void TriangleSurface::draw()
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mTexture->id());

	glBindVertexArray(mVAO);
	glUniformMatrix4fv(mShader.mMatrixUniform, 1, GL_FALSE, glm::value_ptr(mMatrix));
	glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

//void TriangleSurface::construct()
//{
//
//	float xmin = 0.0f, xmax = 1.0f, ymin = 0.0f, ymax = 1.0f, h = 0.25f;
//	for (auto x = xmin; x < xmax; x += h)
//		for (auto y = ymin; y < ymax; y += h)
//		{
//			float z = sin(M_PI * x) * sin(M_PI * y);
//			mVertices.push_back(Vertex{ x,y,z,x,y,z });
//			z = sin(M_PI * (x + h)) * sin(M_PI * y);
//			mVertices.push_back(Vertex{ x + h,y,z,x,y,z });
//			z = sin(M_PI * x) * sin(M_PI * (y + h));
//			mVertices.push_back(Vertex{ x,y + h,z,x,y,z });
//			mVertices.push_back(Vertex{ x,y + h,z,x,y,z });
//			z = sin(M_PI * (x + h)) * sin(M_PI * y);
//			mVertices.push_back(Vertex{ x + h,y,z,x,y,z });
//			z = sin(M_PI * (x + h)) * sin(M_PI * (y + h));
//			mVertices.push_back(Vertex{ x + h,y + h,z,x,y,z });
//		}
//}

void TriangleSurface::construct() 
{
	float triVolume{ 0.0f };

	float xmin = -6.0f, xmax = 40.0f, ymin = -6.0f, ymax = 6.0f, h = 0.4f;
	for (auto x = xmin; x < xmax; x += h)
	{
		for (auto y = ymin; y < ymax; y += h)
		{
			float z = func(x, y);
			mVertices.push_back(Vertex{ x,y,z,x,y,z ,0,0});
			z = func(x + h, y);
			mVertices.push_back(Vertex{ x + h,y,z,x,y,z,1,0 });
			z = func(x, y + h);
			mVertices.push_back(Vertex{ x,y + h,z,x,y,z ,1,1});
			triVolume += (pow(h, 2) / 2) * z;

			mVertices.push_back(Vertex{ x,y + h,z,x,y,z,0,0 });
			z = func(x + h, y);
			mVertices.push_back(Vertex{ x + h,y,z,x,y,z,1,0 });
			z = func(x + h, y + h);
			mVertices.push_back(Vertex{ x + h,y + h,z,x,y,z,1,1 });
			triVolume += (pow(h, 2) / 2) * z;
		}
	}
	writeFile();

	std::fstream in2;
	in2.open("comp1Volume.txt");
	in2 << triVolume << std::endl;
	in2.close();
}

void TriangleSurface::writeFile()
{
	std::fstream in;
	in.open("FileTest.txt");

	in << mVertices.size() << std::endl;

	for (float i = 0; i < mVertices.size(); i++)
	{
		in << "(" << mVertices[i].m_xyz[0] << ", " << mVertices[i].m_xyz[1] << ", " << mVertices[i].m_xyz[2] << ")";
		in << "(" << mVertices[i].m_normal[0] << ", " << mVertices[i].m_normal[1] << ", " << mVertices[i].m_normal[2] << ")";
		in << "(" << mVertices[i].m_uv[0] << ", " << mVertices[i].m_uv[1] << ")" << std::endl;

	}
}

float TriangleSurface::func(float x, float y)
{
	return sin(x + y);
}
