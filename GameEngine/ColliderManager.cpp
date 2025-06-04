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