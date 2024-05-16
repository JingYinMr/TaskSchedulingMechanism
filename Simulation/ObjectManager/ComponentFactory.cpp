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
	//父类纯虚函数没有重写，因此编译报错，后续需要在修改补充

	return nullptr;
}

MotionComponentFactory::~MotionComponentFactory()
{
}
