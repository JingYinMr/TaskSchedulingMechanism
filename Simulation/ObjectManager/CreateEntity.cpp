#include "pch.h"
#include "CreateEntity.h"

CreateEntity::CreateEntity(std::shared_ptr<EntityFactoryBase> factory)
{
	this->Factory = factory;
}

std::shared_ptr<EntityBase> CreateEntity::CreateEntityFactory()
{
	std::shared_ptr<EntityBase> Entity = this->Factory->CreateEntity();
	return Entity;
}
