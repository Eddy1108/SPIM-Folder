#pragma once
#include "glm/glm.hpp"
#include <algorithm>
#include <Core/Vertex.h>
#include <vector>

class SortCoords
{
public:

	static bool compareX(Vertex V1, Vertex V2);
	static bool compareY(Vertex V1, Vertex V2);

	static void SortX(std::vector<Vertex>& points);
	static void SortY(std::vector<Vertex>& points);

	static float FindXMin(std::vector<Vertex>& points);
	static float FindYMin(std::vector<Vertex>& points);

	static float FindXMax(std::vector<Vertex>& points);
	static float FindYMax(std::vector<Vertex>& points);
};

