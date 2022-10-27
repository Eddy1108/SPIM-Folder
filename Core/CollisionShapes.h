#pragma once
#include "glm/glm.hpp"


class AABB;
class CollisionShape
{
public:
	enum class Type {
		None = 0,
		AABB
	};

	const Type mType;
	CollisionShape(Type type);

	virtual bool overlap(CollisionShape* shape);
	static bool overlap(AABB* a, AABB* b);

	glm::vec3 mPosition{ 0.f, 0.f, 0.f };
	bool bDrawBox{ true };
};

class AABB : public CollisionShape
{
public:
	AABB();
	virtual bool overlap(CollisionShape* shape) override;

	glm::vec3 mExtent{ 1.f };

	
};

