#include "ObjectsManager.h"
#include "LObject.h"

ObjectsManager::~ObjectsManager()
{
	//Iterate through every object deleting it
	for (std::vector<LObject*>::iterator itr = _objects.begin(); itr != _objects.end(); ++itr)
	{
		delete* itr;
	}

	//Clears the vector so its Count is equals to 0
	_objects.clear();
}

void ObjectsManager::Update(float deltaTime)
{
	//Iterates through every object and executes its Update in case the object is NOT nullptr
	for (std::vector<LObject*>::iterator itr = _objects.begin(); itr != _objects.end(); ++itr) 
	{
		if(*itr != nullptr)
		{
			(*itr)->Update(deltaTime);
		}
	}
}

void ObjectsManager::DestroyManager()
{
	delete ObjectsManager::GetInstance();
}

void ObjectsManager::AddObject(LObject* newObject)
{
	//Iterates through every object to check if the new object is already in the vector
	for (std::vector<LObject*>::const_iterator itr = _objects.begin(); itr != _objects.end(); ++itr) 
	{
		if (*itr != nullptr && *itr == newObject) 
		{
			return;
		}
	}

	//Add the new object to the vector
	_objects.push_back(newObject);
}

void ObjectsManager::DestroyObject(LObject* object)
{
	std::vector<LObject*>::iterator objectToRemove = _objects.end();

	//Iterates through every object. If it is NOT nullptr and it is equals to the entry object it will be saved to be erased from the _objects vector later
	for (std::vector<LObject*>::iterator itr = _objects.begin(); itr != _objects.end(); ++itr)
	{
		if (*itr != nullptr && *itr == object)
		{
			objectToRemove = itr;
			break;
		}
	}

	//Check if the itr of the object was found
	if (objectToRemove != _objects.end()) 
	{
		/*
		* Erase the object from the vector
		* Delete the erased object
		* It won´t be used anymore so it will avoid any dangling pointer
		*/
		_objects.erase(objectToRemove);
		delete object;
	}
}

void ObjectsManager::EnableObjects(bool enable) const
{
	//Iterate through every object
	for (std::vector<LObject*>::const_iterator itr = _objects.begin(); itr != _objects.end(); ++itr)
	{
		if(*itr != nullptr) //Check if the object is nullptr
		{
			//Enable or disable the object
			(*itr)->SetIsActive(enable);
		}
	}
}
