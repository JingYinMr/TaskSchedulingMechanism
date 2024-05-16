#include "pch.h"
#include<iostream>
#include "MissionComponent.h"

MissionComponent::~MissionComponent()
{
}

void MissionComponent::AddNode(std::shared_ptr<MissionComponentBase> elements)
{
	elementsList.push_back(elements);
}

void MissionComponent::RemoveNode(std::shared_ptr<MissionComponentBase> elements)
{
	elementsList.remove(elements);
}

void MissionComponent::PerformTasks()
{
	//1.处理当前结点

	//2.处理叶子结点
	for (auto &Elemelt : this->elementsList)
	{
		//虚函数的调用
		Elemelt->PerformTasks();
	}
}

std::shared_ptr<ComponentBase> MissionComponent::clone() const
{
	return std::make_shared<MissionComponent>(*this);
}

list<std::shared_ptr<MissionComponentBase>> MissionComponent::GetelementsList()
{
	return this->elementsList;
}

ReconnaissanceMission::~ReconnaissanceMission()
{
}

void ReconnaissanceMission::PerformTasks()
{
	//执行侦察任务
	std::cout << "执行侦察任务" << std::endl;
}

std::shared_ptr<ComponentBase> ReconnaissanceMission::clone() const
{
	return std::make_shared<ReconnaissanceMission>(*this);
}

AttackMission::~AttackMission()
{
}

void AttackMission::PerformTasks()
{
	//执行打击任务
	std::cout << "执行打击任务" << std::endl;
}

std::shared_ptr<ComponentBase> AttackMission::clone() const
{
	return std::make_shared<AttackMission>(*this);
}
