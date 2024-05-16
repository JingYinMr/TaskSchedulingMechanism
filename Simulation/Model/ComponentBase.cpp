#include "pch.h"
#include "ComponentBase.h"

ComponentBase::~ComponentBase()
{
}

void ComponentBase::SetComponentType(size_t componenttype)
{
	this->ComponentType = componenttype;
}

size_t ComponentBase::GetComponentType()
{
	return this->ComponentType;
}
