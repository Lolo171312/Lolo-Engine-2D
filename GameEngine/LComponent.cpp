#include "LComponent.h"

void LComponent::AttachComponent(const LObject* ownerPtr)
{
	_owner = ownerPtr;
}
