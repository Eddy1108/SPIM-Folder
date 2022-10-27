#include "Math/Barycentric.h"

bool Barycentric::barycentricIsInside(glm::vec3 pos)
{
	for (int i = 0; i < 3; i++)
	{
		if (pos[i] < 0.f || pos[i] > 1.f)
			return false;	
	}
	
	return true;
}

glm::vec3 Barycentric::calcBarycentric(glm::vec2 point, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
{

	//Vab & Vac
	glm::vec2 Va = p2 - p1;
	glm::vec2 Vb = p3 - p1;
	glm::vec3 n = glm::cross(glm::vec3(Va.x, Va.y, 0), glm::vec3(Vb.x, Vb.y, 0));
	float areal_123 = glm::length(n);
	glm::vec3 baryc;

	// u
	glm::vec2 p = p2 - point;
	glm::vec2 q = p3 - point;
	n = glm::cross(glm::vec3(p.x, p.y, 0), glm::vec3(q.x, q.y, 0));
	baryc.x = n.z / areal_123;

	// v
	p = p3 - point;
	q = p1 - point;
	n = glm::cross(glm::vec3(p.x, p.y, 0), glm::vec3(q.x, q.y, 0));

	baryc.y = n.z / areal_123;

	// w
	p = p1 - point;
	q = p2 - point;
	n = glm::cross(glm::vec3(p.x, p.y, 0), glm::vec3(q.x, q.y, 0));
	baryc.z = n.z / areal_123;

	return baryc;
}
