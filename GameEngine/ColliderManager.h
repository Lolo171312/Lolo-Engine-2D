#pragma once
#include <vector>

class CCollider;

class ColliderManager
{
public:
	ColliderManager();

	inline void AddColliderCmp(CCollider* newColliderCmp) { _colliderCmps.push_back(newColliderCmp); }

	static ColliderManager* GetInstance();

	inline const std::vector<CCollider*>& GetColliderCmps() { return _colliderCmps; }

private:
	static ColliderManager* _instance;
	std::vector<CCollider*> _colliderCmps;
};