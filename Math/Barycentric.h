#pragma once
#include "glm/glm.hpp"

class Barycentric
{

public:

	float u{ 0 }, v{ 0 }, w{ 0 };
	glm::vec3 BaryCoords;

	static bool barycentricIsInside(glm::vec3 pos);
	static glm::vec3 calcBarycentric(glm::vec2 point, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
};

