#pragma once
#include "LComponent.h"

#include <glm/glm/glm.hpp>

enum ColliderType {
	NONE, CIRCLE, RECT
};

class CCollider : public LComponent
{
public:
	CCollider(ColliderType type);

	virtual void Update(float deltaTime);

	virtual bool Collides(CCollider* other) = 0;
	bool CheckCircleCirle(const glm::vec2& pos1, float radius1, const glm::vec2& pos2, float radius2);
	bool CheckRectRect(const glm::vec2& pos1, const glm::vec2& size1, const glm::vec2& pos2, const glm::vec2& size2);
	bool CheckCircleRect(const glm::vec2& pos1, float radius1, const glm::vec2& pos2, const glm::vec2& size2);

	inline ColliderType GetType() const { return _type; }
	inline float GetRadius() const { return _radius; }
	inline const glm::vec2& GetSize() const { return _size; }

protected:
	ColliderType _type;
	float _radius;
	glm::vec2 _size;
};