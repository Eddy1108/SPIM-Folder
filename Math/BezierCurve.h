#pragma once
#include "Core/VisualObject.h"
#include <vector>
#include "mesh/Kube.h"
#include <glm/glm.hpp>

class BezierCurve : public VisualObject
{
public:

	BezierCurve(Shader& shader);
	~BezierCurve();

	void init() override;
	void draw() override;
	void construct();
	glm::vec3 calcBezier(float t);
	
protected:
	
	std::vector<glm::vec3> mControlPoints;
	std::vector<Kube*> mCubes;
};

