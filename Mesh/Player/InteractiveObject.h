#pragma once
#include "Core/VisualObject.h"

class Heightmap;

class InteractiveObject : public VisualObject
{
public:
	InteractiveObject(Shader& shader);
	~InteractiveObject();

	virtual void init() override;
	virtual void draw() override;
	void move(float x, float y, float z) override;


	bool WMove{ false };
	bool AMove{ false };
	bool SMove{ false };
	bool DMove{ false };
	bool QMove{ false };
	bool EMove{ false };
	bool bBypass{ true };

	Heightmap* mHeightmap{ nullptr };

	




};

