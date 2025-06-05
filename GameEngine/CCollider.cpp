#include "CCollider.h"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "ColliderManager.h"
#include <vector>
#include <iostream>
#include "LObject.h"

CCollider::CCollider(ColliderType type) : _type(type)
{
	ColliderManager::GetInstance()->AddColliderCmp(this);
}

void CCollider::Update(float deltaTime)
{
	const std::vector<CCollider*>& colliders = ColliderManager::GetInstance()->GetColliderCmps();
	for (std::vector<CCollider*>::const_iterator itr = colliders.begin(); itr != colliders.end(); ++itr)
	{
		if ((*itr) != nullptr && (*itr) != this)
		{
			bool collide = Collides((*itr));
			if(collide)
			{
				_owner->OnCollisionEnter((*itr));
			}
		}
	}
}

bool CCollider::CheckCircleCirle(const glm::vec2& pos1, float radius1, const glm::vec2& pos2, float radius2)
{
	const float distance = glm::distance(pos1, pos2);

	return distance < (radius1 + radius2);
}

bool CCollider::CheckRectRect(const glm::vec2& pos1, const glm::vec2& size1, const glm::vec2& pos2, const glm::vec2& size2)
{
	const float xHalfSize1 = size1.x * 0.5f;
	const float yHalfSize1 = size1.y * 0.5f;
	const float xHalfSize2 = size2.x * 0.5f;
	const float yHalfSize2 = size2.y * 0.5f;

	const bool horizontalCollide = pos1.x + xHalfSize1 > pos2.x - xHalfSize2 && pos1.x - xHalfSize1 < pos2.x + xHalfSize2;
	if (horizontalCollide) 
	{
		const bool verticalCollide = pos1.y + yHalfSize1 > pos2.y - yHalfSize2 && pos1.y - yHalfSize1 < pos2.y + yHalfSize2;
		if (verticalCollide) 
		{
			return true;
		}
	}
	

	return false;
}

bool CCollider::CheckCircleRect(const glm::vec2& pos1, float radius1, const glm::vec2& pos2, const glm::vec2& size2)
{
	float recTestX = pos1.x;
	float recTestY = pos1.y;

	const float rectHalfSizeX = size2.x * 0.5f;
	const float rectHalfSizeY = size2.y * 0.5f;

	if (pos1.x < pos2.x - rectHalfSizeX) recTestX = pos2.x < rectHalfSizeX;
	else if (pos1.x > pos2.x + rectHalfSizeX) recTestX = pos2.x + rectHalfSizeX;

	if (pos1.y < pos2.y - rectHalfSizeY) recTestY = pos2.y - rectHalfSizeY;
	else if (pos1.y > pos2.y + rectHalfSizeY) recTestY = pos2.y + rectHalfSizeY;

	if (recTestX == pos1.x && recTestY == pos1.y) return true;

	const float distance = glm::distance(pos1, glm::vec2(recTestX, recTestY));
	if (distance <= radius1) 
	{
		return true;
	}

	return false;
}
