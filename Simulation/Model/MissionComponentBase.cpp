#include "pch.h"
#include "MissionComponentBase.h"

MissionComponentBase::~MissionComponentBase()
{
}

std::shared_ptr<ComponentBase> MissionComponentBase::clone() const
{
	//return std::make_shared<MissionComponentBase>(*this);
	return nullptr;
}
