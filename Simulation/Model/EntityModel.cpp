#include "pch.h"
#include "EntityModel.h"
#include"ModelType.h"

#include "MissionComponent.h"

EntityModel::EntityModel()
	:EntityType(EntityType::ET_Unknown)
{
}

EntityModel::~EntityModel()
{
}

void EntityModel::AddNode(std::shared_ptr<ComponentBase> elements)
{
	elementsList.push_back(elements);
}

void EntityModel::RemoveNode(std::shared_ptr<ComponentBase> elements)
{
	elementsList.remove(elements);
}

void EntityModel::Process()
{
	//����ʵ��
	//std::cout << "����ʵ��" << std::endl;

	//�������ģ��
	for (auto & elemeltComponent : this->elementsList)
	{
		//�����������ģ�飨���ڵ㣩
		if (std::shared_ptr<MissionComponent> missionComponent = std::dynamic_pointer_cast<MissionComponent>(elemeltComponent))
		{
			for (auto & elemeltmissionComponent : missionComponent->GetelementsList())
			{
				elemeltmissionComponent->PerformTasks();
			}
		}
	}

}

std::shared_ptr<EntityBase> EntityModel::clone() const
{
	return std::make_shared<EntityModel>(*this);
}

list<std::shared_ptr<ComponentBase>> EntityModel::GetelementsList()
{
	return this->elementsList;
}

void EntityModel::SetEntityName(std::string entityname)
{
	this->EntityName = entityname;
}

std::string EntityModel::GetEntityName()
{
	return this->EntityName;
}

void EntityModel::SetEntityType(size_t entitytype)
{
	this->EntityType = entitytype;
}

size_t EntityModel::GetEntityType()
{
	return this->EntityType;
}

void EntityModel::SetEntityHandle(size_t entityhandle)
{
	this->EntityHandle = entityhandle;
}

size_t EntityModel::GetEntityHandle()
{
	return this->EntityHandle;
}
