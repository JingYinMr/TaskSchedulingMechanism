#include "pch.h"
#include "MissionFactory.h"

#include "MissionComponent.h"

std::shared_ptr<ComponentBase> ReconnaissanceMissionFactory::CreateComponent()
{
	return std::make_shared<ReconnaissanceMission>();
}

ReconnaissanceMissionFactory::~ReconnaissanceMissionFactory()
{
}

std::shared_ptr<ComponentBase> AttackMissionFactory::CreateComponent()
{
	return std::make_shared<AttackMission>();
}

AttackMissionFactory::~AttackMissionFactory()
{
}
