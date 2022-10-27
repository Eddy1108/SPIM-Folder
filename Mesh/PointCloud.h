#pragma once

#include <math.h>
#include <iostream>

#include <vector>

#include "Core/VisualObject.h"

class PointCloud : public VisualObject
{
public:

	PointCloud(Shader& shader);
	~PointCloud();

	void init() override;
	void draw() override;

	//For ankerskogen
	//long double mScaleMultiplyX{ 1.f };
	//long double mScaleMultiplyY{ 1.f };
	//long double mScaleMultiplyZ{ 0.01f };

	//long double mOffsetX{ 612000 };
	//long double mOffsetY{ 612497 };


	//For testing
	long double mScaleMultiplyX{ 1.f };
	long double mScaleMultiplyY{ 1.f };
	long double mScaleMultiplyZ{ 1.f };

	long double mOffsetX{ 473213.f + 1110 / 2 };
	long double mOffsetY{ 6835647.f + 2110 / 2 };
	long double mOffsetZ{ 1700 };

	//For Rindal
	//long double mScaleMultiplyX{ 1.f };
	//long double mScaleMultiplyY{ 1.f };
	//long double mScaleMultiplyZ{ 0.01f };

	//long double mOffsetX{ 509170 };
	//long double mOffsetY{ 509343 };
};

