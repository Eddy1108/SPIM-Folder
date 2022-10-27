#pragma once
#include "Core/VisualObject.h"

class SceneSwitcher : public VisualObject
{
public:

	SceneSwitcher(Shader& shader, float size = 1.f, float x = 1.f, float y = 1.f, float z = 1.f);
	~SceneSwitcher();

	void init() override;
	void draw() override;
	bool activate(float f = 0) override;
	bool getIsSwitch();
	void reset();

private:
	float mx{ 0 };
	float my{ 0 };
	float mz{ 0 };

	bool bSwitch{ false };


};

