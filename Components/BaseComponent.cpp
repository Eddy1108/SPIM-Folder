#include "BaseComponent.h"

BaseComponent::BaseComponent(VisualObject* parent, bool isActivatedFromStart) : mParent{ parent }, bIsActive{isActivatedFromStart}
{
}