#include "Billboard.h"

Billboard::Billboard(std::string materialName, Camera* cam)
    :VisualObject(materialName)
{
	mCam = cam;

	mVertices.push_back(Vertex(-1.f, -1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f)); //A
	mVertices.push_back(Vertex(-1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f)); //C
	mVertices.push_back(Vertex(1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f)); //D

	mVertices.push_back(Vertex(-1.f, -1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f)); //A
	mVertices.push_back(Vertex(1.f, -1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f)); //B
	mVertices.push_back(Vertex(1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f)); //D

	mMatrix = glm::mat4(1.0f);
	mPosition = glm::vec3(0.f, 0.f, 10.f);

    mTexture = new Texture("../SPIM-Folder/Assets/Texture/jacky.bmp");
}

Billboard::Billboard(std::string materialName, Camera* cam, std::string fileDir)
    : VisualObject(materialName)
{

	mVertices.push_back(Vertex(-1.f, -1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f)); //A
	mVertices.push_back(Vertex(-1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f)); //C
	mVertices.push_back(Vertex(1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f)); //D

	mVertices.push_back(Vertex(-1.f, -1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f)); //A
	mVertices.push_back(Vertex(1.f, -1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f)); //B
	mVertices.push_back(Vertex(1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f)); //D

	mMatrix = glm::mat4(1.0f);
	mPosition = glm::vec3(0.f, 0.f, 10.f);

	mCam = cam;
	mTexture = new Texture(fileDir);
}

Billboard::~Billboard()
{
}

void Billboard::init()
{
	VisualObject::initTexture();
}

void Billboard::draw()
{
	//move();

	//RotateToCamMatrix();
	RotateToCamDirect();

	//Not needed but to overwrite other textures in this slot we must do this each draw call
    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, mTexture->id());

	//mMaterial->UpdateUniforms(&mMatrix);

	VisualObject::draw();

}

void Billboard::move()
{

	mMatrix[3].x = mPosition.x;
	mMatrix[3].y = mPosition.y;
	mMatrix[3].z = mPosition.z;

}

void Billboard::RotateToCamMatrix()
{
	billView = glm::inverse(mCam->mVMatrix);

	glm::vec4 leftVectorX = billView[0];
	glm::vec4 upVectorY = billView[1];
	glm::vec4 forwardVectorZ = billView[2];

	mMatrix[0] = leftVectorX;
	mMatrix[1] = upVectorY;
	mMatrix[2] = forwardVectorZ;

	//std::cout << "Cam Pos: " << mCam->getPos().x << ", " << mCam->getPos().y << ", " << mCam->getPos().z << std::endl;
}

void Billboard::RotateToCamDirect()
{
	billView = glm::lookAt(mPosition, mCam->mPosition, mCam->mUp);

	billView = glm::inverse(billView);

	glm::vec4 leftVectorX = billView[0];
	glm::vec4 upVectorY = billView[1];
	glm::vec4 forwardVectorZ = billView[2];

	mMatrix[0] = leftVectorX;
	mMatrix[1] = upVectorY;
	mMatrix[2] = forwardVectorZ;
}
