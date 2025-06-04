#pragma once

class LObject;

class LComponent
{
public:
	/*Component Functions*/
	/*
	* This function is called when the Component is attached to an LObject
	*/
	virtual void AttachComponent(const LObject* ownerPtr);

	/*
	* This function is called every frame is the component is active
	*/
	virtual void Update(float deltaTime) = 0;

	/*
	* Function to modify _isActive variable
	* TRUE: Update function will be called every frame
	* FALSE: Update function won´t be called at any time
	*/
	inline void SetIsActive(bool active) { _isActive = active; }

	/*
	* Getter for _isActive variable
	*/
	inline bool GetIsActive() const { return _isActive; }

	inline const LObject* GetOwner() const { return _owner; }

private:
	const LObject* _owner;
	bool _isActive = true;
};