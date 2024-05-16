#include "pch.h"
#include "ComponentFactory.h"

#include "MissionComponent.h"
#include "MotionComponent.h"

ComponentFactoryBase::~ComponentFactoryBase()
{
}

std::shared_ptr<ComponentBase> MissionComponentFactory::CreateComponent()
{
	return std::make_shared<MissionComponent>();
}

MissionComponentFactory::~MissionComponentFactory()
{
}

std::shared_ptr<ComponentBase> MotionComponentFactory::CreateComponent()
{
	//return std::make_shared<MotionComponent>();
	//���ി�麯��û����д����˱��뱨��������Ҫ���޸Ĳ���

	return nullptr;
}

MotionComponentFactory::~MotionComponentFactory()
{
}
