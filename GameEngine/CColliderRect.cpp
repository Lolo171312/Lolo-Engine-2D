#include "CColliderRect.h"
#include "LObject.h"

bool CColliderRect::Collides(CCollider* other)
{
    if (other == nullptr) return false;

    if (other->GetType()  == RECT)
    {
        return CheckRectRect(GetOwner()->GetObjectLocation(), _size, other->GetOwner()->GetObjectLocation(), other->GetSize());
    }
    else if (other->GetType() == CIRCLE) 
    {
        return CheckCircleRect(other->GetOwner()->GetObjectLocation(), other->GetRadius(), GetOwner()->GetObjectLocation(), _size);
    }

    return false;
}
