#pragma once

#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include "qvector3d.h"

class Vertex
{
	friend std::ostream& operator<< (std::ostream&, const Vertex&);

	//! Overloaded ostream operator which reads all vertex data from an open textfile stream
	friend std::istream& operator>> (std::istream&, Vertex&);

public:
	float m_xyz[3];	// 3D position
	float m_normal[3];	//normal in 3D or rgb colors
	float m_uv[2]; //UV

	Vertex();
	Vertex(float x, float y, float z, float r, float g, float b, float u = 0, float v = 1);
	
	QVector3D getPos();

	glm::vec3 getXYZ();


};

