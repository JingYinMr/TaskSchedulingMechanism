#include "pch.h"
#include<iostream>
#include "EntityFactory.h"

#include<EntityModel.h>

EntityFactoryBase::~EntityFactoryBase()
{
}

std::shared_ptr<EntityBase> EntityFactory::CreateEntity()
{
	return std::make_shared<EntityModel>();
}

EntityFactory::~EntityFactory()
{
}
