#include "LComponent.h"

void LComponent::AttachComponent(LObject* ownerPtr)
{
	_owner = ownerPtr;
}
