#include "pch.h"
#include "CreateComponent.h"

CreateComponent::CreateComponent(std::shared_ptr<ComponentFactoryBase> factory)
{
	this->Factory = factory;
}

std::shared_ptr<ComponentBase> CreateComponent::CreateComponentFactory()
{
	std::shared_ptr<ComponentBase> Component = this->Factory->CreateComponent();
	return Component;
}
