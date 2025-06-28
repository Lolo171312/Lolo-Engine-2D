#pragma once

//#include "../SingletonBase.h"
#include <unordered_map>

template <class T>
class BaseFactory// : public SingletonBase<BaseFactory<T>>
{
public:
	~BaseFactory();
	T* GetType(const char* typeKey);
	virtual void DestroyFactory() = 0;

private:
	virtual T* InsertNewType(const char* key)
	{
		return nullptr;
	};

	std::unordered_map<const char*, T*> _types;
};

template<class T>
inline BaseFactory<T>::~BaseFactory()
{
	for (typename std::unordered_map<const char*, T*>::iterator itr = _types.begin(); itr != _types.end(); ++itr)
	{
		if ((*itr).second != nullptr)
		{
			delete (*itr).second;
		}
	}
}

template<class T>
inline T* BaseFactory<T>::GetType(const char* typeKey)
{
	typename std::unordered_map<const char*, T*>::iterator itr = _types.find(typeKey);
	if (itr == _types.end())
	{
		T* newType = InsertNewType(typeKey);
		_types.insert(std::pair<const char*, T*>(typeKey, newType));

		return newType;
	}

	return (*itr).second;
}