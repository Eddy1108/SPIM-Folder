#include "Vertex.h"
#include "qvector3d.h"

std::ostream& operator<< (std::ostream& os, const Vertex& v) {
	os << std::fixed;
	os << "(" << v.m_xyz[0] << ", " << v.m_xyz[1] << ", " << v.m_xyz[2] << ") ";
	os << "(" << v.m_normal[0] << ", " << v.m_normal[1] << ", " << v.m_normal[2] << ") ";
	os << "(" << v.m_uv[0] << ", " << v.m_uv[1] << ") ";
	return os;
}
std::istream& operator>> (std::istream& is, Vertex& v) {
	// Trenger fire temporære variabler som kun skal lese inn parenteser og komma
	char dum, dum2, dum3, dum4;
	is >> dum >> v.m_xyz[0] >> dum2 >> v.m_xyz[1] >> dum3 >> v.m_xyz[2] >> dum4;
	is >> dum >> v.m_normal[0] >> dum2 >> v.m_normal[1] >> dum3 >> v.m_normal[2] >> dum4;
	is >> dum >> v.m_uv[0] >> dum2 >> v.m_uv[1] >> dum3;
	return is;

}

Vertex::Vertex()
{
}

Vertex::Vertex(float x, float y, float z, float r, float g, float b, float u, float v)
{
	m_xyz[0] = x;
	m_xyz[1] = y;
	m_xyz[2] = z;

	m_normal[0] = r;
	m_normal[1] = g;
	m_normal[2] = b;

	m_uv[0] = u;
	m_uv[1] = v;

}

glm::vec3 Vertex::getXYZ()
{
	return glm::vec3(m_xyz[0], m_xyz[1], m_xyz[2]);
}

QVector3D Vertex::getPos()
{
	return QVector3D(m_xyz[0], m_xyz[1], m_xyz[2]);
}
