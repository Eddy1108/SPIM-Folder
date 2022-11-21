#include "VisualObject.h"
#include "materiallist.h"
#include "renderwindow.h"


VisualObject::VisualObject(std::string materialName) : mMaterialName(materialName)
{
    setMaterial(materialName);
}

VisualObject::~VisualObject() {
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
}

void VisualObject::init()
{
	initializeOpenGLFunctions();

	//Vertex Array Object - VAO
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	//Vertex Buffer Object to hold vertices - VBO
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	//EBO
    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

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

void VisualObject::initTexture()
{
	///Init set up for textures
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

	//init Components here
	StartAudio();
}

void VisualObject::draw()
{
    mMaterial->UpdateUniforms(&mMatrix);
    //glUniformMatrix4fv(mShader.mMatrixUniform, 1, GL_FALSE, glm::value_ptr(mMatrix));
    glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, mVertices.size());


	//Render Components here
    UpdateAudio();
}

void VisualObject::setMaterial(std::string materialName)
{
    MaterialList* matList = RenderWindow::mMaterialList;
    if(!matList){
        std::cout << "Error, materialList was never compiled in renderWindow!\n";
        return;
    }
    for(int i = 0; i < matList->mMaterialSize; i++){
        if(matList->mMaterial[i].first == materialName && !materialName.empty()){
            mMaterial = matList->mMaterial[i].second;
            break;
        }
    }
}

/*void VisualObject::setMaterial(MaterialList *materialList)
{
    //Gets material lsit
    MaterialList* matList = RenderWindow::mMaterialList;
    if(!matList){
        std::cout << "Error, materialList was never compiled in renderWindow!\n";
        return;
    }
    //Sets material
    for(int i = 0; i < materialList->mMaterialSize; i++){
        if(matList->mMaterial[i].first == mMaterialName && !mMaterialName.empty()){
            mMaterial = materialList->mMaterial[i].second;
            break;
        }
    }
    if(!mMaterial)
        std::cout << "Error: VisualObject's ShaderProgram is not found! mShaderProgramName might be spelled incorrect or not given";
}

void VisualObject::setMaterial(Material &material)
{
    mMaterial = &material;
}*/

void VisualObject::move(float x, float y, float z)
{
	//This is a hard set move, no delta.

	mMatrix = glm::mat4(1.0f);		//Always do this first before moving	-> Always make sure mMatrix has been set to identity at least once before, if not its invisible
	glm::vec3 pos = glm::vec3(x, y, z);
	mPosition = pos;
	mMatrix = glm::translate(mMatrix, pos);

	if (mBShape)
	{
		mBShape->mPosition = mPosition;
	}
}

glm::vec2 VisualObject::getPosition2D()
{
	return glm::vec2{ mPosition.x, mPosition.y };
}

glm::vec3 VisualObject::getPosition3D()
{
	return mPosition;
}

void VisualObject::setName(std::string name)
{
	mName = name;
}

std::string VisualObject::getName() const
{
	return mName;
}

void VisualObject::StartAudio()
{
	if (AudioVec.size() > 0)
	{
		for (int i = 0; i < AudioVec.size(); i++)
		{
			AudioVec[i]->PlayAudio(1.f,1.f,true, mPosition);
		}
	}
}

void VisualObject::UpdateAudio()
{
	if (AudioVec.size() > 0)
	{
		for (int i = 0; i < AudioVec.size(); i++)
		{
			AudioVec[i]->UpdateAudio(1.f, 1.f, mPosition);
			AudioVec[i]->UpdateListener(RenderWindow::mCurrentCamera->getPos(), glm::vec3{ 0.f,0.f,0.f }, RenderWindow::mCurrentCamera->GetUp(), RenderWindow::mCurrentCamera->GetForward());
		}
	}
}

