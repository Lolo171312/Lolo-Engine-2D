#include "ColliderManager.h"

ColliderManager* ColliderManager::_instance = nullptr;

ColliderManager::ColliderManager()
{
	_instance = this;
}

ColliderManager* ColliderManager::GetInstance()
{
	if (_instance == nullptr) 
	{
		_instance = new ColliderManager();
	}

	return _instance;
}

void ColliderManager::Destroy()
{
	ColliderManager* ptr = ColliderManager::GetInstance();
	if(ptr)
	{
		delete ptr;
	}
}
