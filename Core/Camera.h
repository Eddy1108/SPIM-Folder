#pragma once
#include <QOpenGLFunctions_4_1_Core>
#include <qmatrix4x4.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"

class Camera : public QOpenGLFunctions_4_1_Core
{
private:
	
	glm::vec3 mEye = glm::vec3(0.f,0.f,0.f);	//Not needed??

	GLint mPMatrixUniform{ 0 };
	GLint mVMatrixUniform{ 0 };


public:

	glm::mat4 mVMatrix = glm::mat4{ 1.0f };
	glm::mat4 mPMatrix = glm::mat4{ 1.0f };	//Set to identity

	glm::vec3 mUp = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 mOrientation = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 mPosition = glm::vec3(0.f, 0.f, 250.f); //Default Position

	Camera();
	void init(GLint pMatrixUniform, GLint vMatrixUniform);
	void perspective(float degrees, float aspect, float nearplane, float farplane);
	void lookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
	void update();
	

	glm::vec3 getPos() { return mPosition; };
	glm::vec3 GetUp() { return mUp; };
	glm::vec3 GetForward() { return mOrientation; };

	//glm::vec3 GetUpWorld() { return mPosition + mUp; };
	//glm::vec3 GetForwardWorld() { return mPosition + mOrientation; };
	glm::vec3 GetUpWorld() { return glm::vec3(mVMatrix[0][0], mVMatrix[1][0], mVMatrix[2][0]); };
	glm::vec3 GetForwardWorld() { return glm::vec3(mVMatrix[0][1], mVMatrix[1][1], mVMatrix[2][1]); };


	//New camera stuff
	bool bFollowPlayer{ false };
	float speed = 1.f;
	float sensitivity = 0.4f;
	void TakeInput();

	float mouseX{ 0 };
	float mouseY{ 0 };
	float mouseOldX{ 0 };
	float mouseOldY{ 0 };

	bool WMove{ false };
	bool AMove{ false };
	bool SMove{ false };
	bool DMove{ false };
	bool QMove{ false };
	bool EMove{ false };

	bool MouseMove{ false };
};