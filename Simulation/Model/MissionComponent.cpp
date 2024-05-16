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
	//1.����ǰ���

	//2.����Ҷ�ӽ��
	for (auto &Elemelt : this->elementsList)
	{
		//�麯���ĵ���
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
	//ִ���������
	std::cout << "ִ���������" << std::endl;
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
	//ִ�д������
	std::cout << "ִ�д������" << std::endl;
}

std::shared_ptr<ComponentBase> AttackMission::clone() const
{
	return std::make_shared<AttackMission>(*this);
}
