#include "Player.h"
#include "mesh/Heightmap.h"

#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"

Player::Player(Shader& shader)
	: InteractiveObject(shader)
{
	//Player model from here : https://sketchfab.com/3d-models/among-us-428bb9a3637e458c8336e4a7aefd4e3d
	PlayerModel = new ObjLoader(shader, "../3DProgExam/Assets/models/crew.obj", "../3DProgExam/Assets/tex/pew.bmp");
	CameraModel = new ObjLoader(shader, "../3DProgExam/Assets/models/camera2.obj", "../3DProgExam/Assets/tex/gray.bmp");

	mBShape = new AABB();
}

void Player::init()
{
	initializeOpenGLFunctions();

	PlayerModel->init();
	CameraModel->init();

}

void Player::draw()
{
	//CheckWinLose();

	//PlayerModel->draw();
	//
	//updateFakeCam();

	//if (bDrawCam)
	//{
	//	//CameraModel->draw();
	//}
	//if (StatusScreen)
	//{
	//	//StatusScreen->draw();
	//}
}

void Player::move(float x, float y, float z)
{
	rotate = 0;

	if (!bDrawCam)	//Stop moving when the editor cam is being drawn, so editor mode = no move.
	{

		if (bStunned)
		{
			TimeEnd = std::chrono::steady_clock::now();

			auto test = std::chrono::duration_cast<std::chrono::seconds>(TimeEnd - TimeStart).count();
			//std::cout << "STUN Start: " << test << std::endl;

			if (test > 1.f)
			{
				bStunned = false;
			}
			else
				return;
		}

		//Using forward vector to move!
		if (SMove) mPosition += mSpeed * -mForward;

		//If colliding with a fence, the player can only go backwards, BUG: Player can walk backwards through fences :)
		if (!bBlocked)
		{
			if (WMove) mPosition += mSpeed * mForward;

			if (AMove) rotate = 3;

			if (DMove) rotate = -3;
		}


		//Get height from heightmap if it is present (which it should be)
		if (mHeightmap && mHeightmap->IsInside(mPosition))
		{
			//mz = mHeightmap->getHeight(glm::vec3(mx, my, mz));
			mPosition.z = mHeightmap->getHeight(mPosition);
		}

		//Rotate the player! variable "rotate" changes based on what input is, defaul is 0 so no turning unless unput is gotten
		mMatrix = glm::rotate(mMatrix, glm::radians(rotate), glm::vec3{ 0.f,0.f,1.f });

		mMatrix[3].x = mPosition.x;
		mMatrix[3].y = mPosition.y;
		mMatrix[3].z = mPosition.z;

		mForward = glm::rotate(mForward, glm::radians(rotate), mUp);


		//Final stuff dont mind this
		if (mBShape)
		{
			mBShape->mPosition = mPosition;
			mBShape->mPosition.z += 0.8f;
		}
		PlayerModel->mMatrix = mMatrix;

		mCameraOffset = glm::vec3{ mPosition.x + -mForward.x * 10.f, mPosition.y + -mForward.y * 10.f, mPosition.z + 10.f };

		if (StatusScreen)
		{
			StatusScreen->mPosition = mPosition + (mForward * glm::vec3(2.f));
			StatusScreen->mPosition.z += 5.f;

		}

		//Reset is player is blocked;
		bBlocked = false;
	}
}

void Player::updateFakeCam()
{

	CameraModel->mMatrix[3].x = mCameraOffset.x; CameraModel->mMatrix[3].y = mCameraOffset.y; CameraModel->mMatrix[3].z = mCameraOffset.z;
	CameraModel->mMatrix = glm::rotate(CameraModel->mMatrix, glm::radians(rotate), glm::vec3{ 0.f,0.f,1.f });

}

void Player::CollectTrophy()
{
	++mTrophies;
	std::cout << "Player Trophies: " << mTrophies << std::endl;

	if (mTrophies == 10)
	{
		WinState = 1;
	}
}

void Player::CheckWinLose()
{
	if (WinState != 0 && !bGameOver)
	{
		if (WinState == 1)
		{
			StatusScreen = new Billboard(mShader, mCam, "../3DProgExam/Assets/tex/win.bmp");
			StatusScreen->init();
		}
		else if (WinState == 2)
		{
			StatusScreen = new Billboard(mShader, mCam, "../3DProgExam/Assets/tex/lost.bmp");
			StatusScreen->init();
		}

		bGameOver = true;
	}
}

bool Player::activate(float f)	//activate is called when colliding with bombs
{
	if (!bStunned)
	{
		bStunned = true;
		TimeStart = std::chrono::steady_clock::now();
		std::cout << "PLAYER STUNNED" << std::endl;

		return true;
	}
	else
		return false;
}
