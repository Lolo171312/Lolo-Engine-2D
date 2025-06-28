#pragma once
template <class T>
class SingletonBase
{
public:
	static T* Init();
	static T* GetInstance();

private:
	static T* _instance;
};

template <typename T>
T* SingletonBase<T>::_instance = nullptr;

template<class T>
inline T* SingletonBase<T>::Init()
{
	_instance = new T();
	return _instance;
}

template<class T>
inline T* SingletonBase<T>::GetInstance()
{
	if(_instance == nullptr)
	{
		Init();
	}
	return _instance;
}
