#include "Camera.h"
#include <iostream>

Camera::Camera()
{
}

void Camera::init(GLint pMatrixUniform, GLint vMatrixUniform)
{
	mPMatrixUniform = pMatrixUniform;
	mVMatrixUniform = vMatrixUniform;
}

void Camera::perspective(float degrees, float aspect, float nearplane, float farplane)
{
	mPMatrix = glm::perspective(glm::radians(degrees), aspect, nearplane, farplane);
}

void Camera::lookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	if (!bFollowPlayer)
    {
		mVMatrix = glm::lookAt(mPosition, mPosition + mOrientation, mUp);
	}
	else
	{
		mPosition = eye;
		mVMatrix = glm::lookAt(eye, at, up);
		
	}

	update();
}

void Camera::update()
{
    Move();

	initializeOpenGLFunctions();
	glUniformMatrix4fv(mPMatrixUniform, 1, GL_FALSE, glm::value_ptr(mPMatrix));
	glUniformMatrix4fv(mVMatrixUniform, 1, GL_FALSE, glm::value_ptr(mVMatrix));

}

void Camera::mouseMovementStop()
{
    mLastMousePos = glm::vec2(mouseX,mouseY);
    MouseMove = false;
}

void Camera::mouseMovementStart(float eventX, float eventY)
{
    // Mouse starting position
    mouseX = eventY;
    mouseY = eventX;
    mouseOldX = mouseX;
    mouseOldY = mouseY;

    MouseMove = true;
}

void Camera::Move()
{
	//Button Inputs
	if (WMove)
	{
		mPosition += speed * mOrientation;
	}
	if (AMove)
	{
		mPosition += speed * -glm::normalize(glm::cross(mOrientation, mUp));
	}
	if (SMove)
	{
		mPosition += speed * -mOrientation;
	}
	if (DMove)
	{
		mPosition += speed * glm::normalize(glm::cross(mOrientation, mUp));
	}
	if (QMove)
	{
        mPosition += speed * -mUp;
	}
	if (EMove)
	{
        mPosition += speed * mUp;
	}

	//Mouse Input
	if (MouseMove)
    {
        float mouseDeltaX{0};
        float mouseDeltaY{0};

        mouseDeltaX = mouseOldX - mouseX;
        mouseDeltaY = mouseOldY - mouseY;

		mouseOldX = mouseX;
		mouseOldY = mouseY;

		float rotX = mouseDeltaX * sensitivity;
		float rotY = mouseDeltaY * sensitivity;

		glm::vec3 newOrientation = glm::rotate(mOrientation, glm::radians(rotX), glm::normalize(glm::cross(mOrientation, mUp)));

		if (abs(glm::angle(newOrientation, mUp) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			mOrientation = newOrientation;
		}

		mOrientation = glm::rotate(mOrientation, glm::radians(rotY), mUp);
	}
}
