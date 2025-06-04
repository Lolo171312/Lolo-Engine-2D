#include "CColliderCircle.h"
#include "LObject.h"

bool CColliderCircle::Collides(CCollider* other)
{
    if (other == nullptr) return false;

    if (other->GetType() == CIRCLE) 
    {
        return CheckCircleCirle(GetOwner()->GetObjectLocation(), this->GetRadius(),
            other->GetOwner()->GetObjectLocation(), other->GetRadius());
    }
    else if (other->GetType() == RECT) 
    {
        return CheckCircleRect(GetOwner()->GetObjectLocation(), this->GetRadius(),
            other->GetOwner()->GetObjectLocation(), other->GetSize());
    }

    return false;
}
