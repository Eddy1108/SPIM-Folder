#pragma once
#include "Mesh/Player/InteractiveObject.h"
#include "Mesh/ObjLoader.h"
#include "Mesh/Billboard.h"

#include <chrono>

class Player : public InteractiveObject
{
public:
	Player(Shader& shader);

	void init() override;
	void draw() override;
	void move(float x, float y, float z) override;

	void updateFakeCam();
	void CollectTrophy();
	void CheckWinLose();

	//Activate STUN!
	bool activate(float f = 0) override;

	glm::vec3 mForward{ 0.f, 1.f, 0.f };
	glm::vec3 mUp{ 0.f,0.f,1.f };

	
	int WinState{ 0 };	//0 = Playing, 1 = WIN, 2 = LOSE

	ObjLoader* PlayerModel{ nullptr };
	ObjLoader* CameraModel{ nullptr };
	Billboard* StatusScreen{ nullptr };

	Camera* mCam{ nullptr };

	bool bGameOver{ false };
	bool bDrawCam{ true };
	bool bBlocked{ false };

private:
	float rotate{ 0 };
	float mSpeed = 0.25f;

	int mTrophies{ 0 };

	bool bStunned{ false };

	std::chrono::steady_clock::time_point TimeStart;
	std::chrono::steady_clock::time_point TimeEnd;
};

