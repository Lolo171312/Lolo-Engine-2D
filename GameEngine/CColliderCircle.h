#pragma once
#include "CCollider.h"

class CColliderCircle : public CCollider
{
public:
	CColliderCircle(float radius) : CCollider(CIRCLE)
	{
		_radius = radius;
	}

	virtual bool Collides(CCollider* other);
};