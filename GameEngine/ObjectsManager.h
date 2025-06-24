#pragma once
#include "SingletonBase.h"
#include <vector>

class LObject;

//This class inherits from SingletonBase so there will only be one instance of this class
//It will manage every object, calling their Update and being able to destroy o add objects
//It will destroy every object at the end of the program
class ObjectsManager : public SingletonBase<ObjectsManager>
{
public:
	//Destroy every object
	~ObjectsManager();

	//Execute Update of every object
	void Update(float deltaTime);
	//Destroy the Manager so it can destroy every object
	void DestroyManager();

	//Add or Destroy an object of _objects vector
	void AddObject(LObject* newObject);
	void DestroyObject(LObject* object);

private:
	//Holds every Object added to the manager
	std::vector<LObject*> _objects;
};

