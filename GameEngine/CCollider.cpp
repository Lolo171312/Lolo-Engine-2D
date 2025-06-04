#include "CCollider.h"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "ColliderManager.h"
#include <vector>
#include <iostream>

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
			if (collide) 
			{
				std::cout << "Collides: TRUE" << std::endl;
			}
			else 
			{
				std::cout << "Collides: FALSE" << std::endl;
			}
		}
	}
}

bool CCollider::CheckCircleCirle(const glm::vec2& pos1, float radius1, const glm::vec2& pos2, float radius2)
{
	float distance = glm::distance(pos1, pos2);

	return distance < (radius1 + radius2);
}

bool CCollider::CheckRectRect(const glm::vec2& pos1, const glm::vec2& size1, const glm::vec2& pos2, const glm::vec2& size2)
{
	bool xCoords = pos1.x + size1.x >= pos2.x && pos1.x <= pos2.x + size2.x;
	bool yCoords = pos1.y + size1.y >= pos2.y && pos1.y <= pos2.y + size2.y;

	return xCoords && yCoords;
}

bool CCollider::CheckCircleRect(const glm::vec2& pos1, float radius1, const glm::vec2& pos2, const glm::vec2& size2)
{


	return false;
}
