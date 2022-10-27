#include "Core/CollisionShapes.h"
#include <iostream>

CollisionShape::CollisionShape(Type type)
	: mType{type}
{
}

bool CollisionShape::overlap(CollisionShape* shape)
{
	std::cout << "THIS IS NOT SUPPOSED TO BE USED" << std::endl;
	return false;
}

bool CollisionShape::overlap(AABB* a, AABB* b)
{
	glm::vec3 aPosition = a->mPosition;
	glm::vec3 aExtent = a->mExtent;

	glm::vec3 bPosition = b->mPosition;
	glm::vec3 bExtent = b->mExtent;

	glm::vec3 distance{
		abs(aPosition.x - bPosition.x),
		abs(aPosition.y - bPosition.y),
		abs(aPosition.z - bPosition.z)
	};

	glm::vec3 minDistance{
		aExtent.x + bExtent.x,
		aExtent.y + bExtent.y,
		aExtent.z + bExtent.z
	};

	return (
		distance.x < minDistance.x && distance.y < minDistance.y && distance.z < minDistance.z
		);


}

AABB::AABB()
	:CollisionShape(Type::AABB)
{
}

bool AABB::overlap(CollisionShape* shape)
{
	switch (shape->mType)
	{
	case Type::AABB:
		return CollisionShape::overlap(this, dynamic_cast<AABB*>(shape));
	default:
		return CollisionShape::overlap(this, dynamic_cast<AABB*>(shape));
	}
}
