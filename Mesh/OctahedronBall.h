#pragma once
#include "Core/VisualObject.h"
class Vertex;

class OctahedronBall : public VisualObject
{
private:
	int m_rekursjoner;
	int m_indeks;
	void lagTriangel(const QVector3D& v1, const QVector3D& v2, const QVector3D& v3);
	void subDivide(const QVector3D& a, const QVector3D& b, const QVector3D& c, int n);
	void oktaederUnitBall();

public:
	OctahedronBall(Shader& shader, int n = 0);
	~OctahedronBall();

	void init() override;
	void draw() override;
};

