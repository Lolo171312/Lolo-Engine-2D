#pragma once

class LObject;

class LComponent
{
public:
	virtual ~LComponent()
	{}

	/*Component Functions*/
	/*
	* This function is called when the Component is attached to an LObject
	*/
	virtual void AttachComponent(LObject* ownerPtr);

	/*
	* This function is called every frame is the component is active
	*/
	virtual void Update(float deltaTime) = 0;

	/*
	* Function to modify _isActive variable
	* TRUE: Update function will be called every frame
	* FALSE: Update function won´t be called at any time
	*/
	inline void SetIsEnabled(bool enabled) { _isEnabled = enabled; }

	/*
	* Getter for _isActive variable
	*/
	inline bool GetIsEnabled() const { return _isEnabled; }

	inline LObject* GetOwner() const { return _owner; }

protected:
	LObject* _owner;
	bool _isEnabled = true;
};