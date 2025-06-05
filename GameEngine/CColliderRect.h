#pragma once
#include "CCollider.h"
class CColliderRect : public CCollider
{
public:
	CColliderRect(float width, float height) : CCollider(RECT)
	{
		_size = glm::vec2(width, height);
	}

	virtual bool Collides(CCollider* other);
};