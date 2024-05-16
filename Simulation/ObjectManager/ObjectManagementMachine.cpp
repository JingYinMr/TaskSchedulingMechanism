#include "pch.h"
#include "ObjectManagementMachine.h"

#include "CreateEntity.h"
#include "CreateComponent.h"
#include "MissionComponent.h"
#include "EntityModel.h"


ObjectManagementMachine::~ObjectManagementMachine()
{
	if (!this->EntityVec.empty())
	{
		this->EntityVec.clear();
	}
}

ObjectManagementMachine * ObjectManagementMachine::Instance()
{
	static ObjectManagementMachine object;//��̬����
	return &object;//���ض������ڵ�ַ
}

vector<std::shared_ptr<EntityBase>> ObjectManagementMachine::GetObjectManger()
{
	return this->EntityVec;
}

vector<std::shared_ptr<EntityBase>> ObjectManagementMachine::GetEntityModelVec()
{
	return this->EntityModelVec;
}

vector<std::shared_ptr<ComponentBase>> ObjectManagementMachine::GetComponentModelVec()
{
	return this->ComponentModelVec;
}

std::vector<std::shared_ptr<EntityBase>> ObjectManagementMachine::GetBattleSpaceVec()
{
	return this->BattleSpaceVec;
}

void ObjectManagementMachine::CreateObject(std::shared_ptr<EntityFactoryBase> EntityFactory)
{
	CreateEntity createEntity(EntityFactory);
	std::shared_ptr<EntityBase> Entity = createEntity.CreateEntityFactory();
	this->EntityModelVec.push_back(Entity);
}

void ObjectManagementMachine::CreateObject(std::shared_ptr<ComponentFactoryBase> ComponentFactory)
{	
	//��Ҫ�����������
	if (std::shared_ptr<MissionComponentFactory> missionComponentFactory = std::dynamic_pointer_cast<MissionComponentFactory>(ComponentFactory))
	{
		//��������������û���������������һ����������������������������
		if (!IsMissionComponent)
		{
			//�����������
			std::shared_ptr<MissionComponentFactory> missionComponentFactory = std::make_shared<MissionComponentFactory>();
			CreateComponent createComponent(missionComponentFactory);
			std::shared_ptr<ComponentBase> missionComponent = createComponent.CreateComponentFactory();

			//�������ģ������� ���ڹ��������������
			this->ComponentModelVec.push_back(missionComponent);

			IsMissionComponent = true;
		}
	}

	for (auto & elemelt : this->ComponentModelVec)
	{
		//��ȡ�������
		if (std::shared_ptr<MissionComponent> missionComponent = std::dynamic_pointer_cast<MissionComponent>(elemelt))
		{
			CreateComponent createComponent(ComponentFactory);
			std::shared_ptr<ComponentBase> Mission = createComponent.CreateComponentFactory();
				
			//���������񣬷������������
			missionComponent->AddNode(std::dynamic_pointer_cast<MissionComponentBase>(Mission));

			return;
		}
	}
}

std::shared_ptr<EntityBase> ObjectManagementMachine::cloneEntity(const std::shared_ptr<EntityBase>& EntityBasePtr)
{
	if (EntityBasePtr) 
	{
		// ���ö���� clone �����õ����������ָ��
		std::shared_ptr<EntityBase> NewEntity = EntityBasePtr->clone();

		this->EntityVec.push_back(NewEntity);

		return NewEntity;
	}
	return nullptr; // ��������ָ��Ϊ�գ��򷵻ؿ�����ָ��
}

std::shared_ptr<ComponentBase> ObjectManagementMachine::cloneComponent(const std::shared_ptr<ComponentBase>& ComponentBasePtr)
{
	if (ComponentBasePtr)
	{
		// ���ö���� clone �����õ����������ָ��
		std::shared_ptr<ComponentBase> NewComponent = ComponentBasePtr->clone();

		return NewComponent;
	}
	return nullptr; // ��������ָ��Ϊ�գ��򷵻ؿ�����ָ��
}

void ObjectManagementMachine::EntityCombineComponent(std::shared_ptr<EntityBase>& EntityBasePtr, const std::shared_ptr<ComponentBase>& ComponentBasePtr)
{
	std::shared_ptr<ComponentBase> NewComponent = cloneComponent(ComponentBasePtr);

	//�жϸý���Ƿ�Ϊ������
	if (std::shared_ptr<MissionComponentBase> missionComponentBase = std::dynamic_pointer_cast<MissionComponentBase>(NewComponent))
	{
		//ʵ�彫����ת��Ϊ����
		if (std::shared_ptr<EntityModel> entityModel = std::dynamic_pointer_cast<EntityModel>(EntityBasePtr))
		{
			//ѭ������ʵ�����Ϲ��ص������������
			for (auto & Elemelt : entityModel->GetelementsList())
			{
				//�жϵ�ǰʵ���Ƿ�������������ڵ㣩
				if (std::shared_ptr<MissionComponent> missionComponent = std::dynamic_pointer_cast<MissionComponent>(Elemelt))
				{
					missionComponent->AddNode(missionComponentBase);

					return;
				}
			}

			//û������������ʹ���һ�������������
			{
				//�����������
				std::shared_ptr<MissionComponentFactory> missionComponentFactory = std::make_shared<MissionComponentFactory>();
				CreateComponent createComponent(missionComponentFactory);
				std::shared_ptr<ComponentBase> missionComponent = createComponent.CreateComponentFactory();

				//�������ģ������� ���ڹ��������������
				entityModel->AddNode(missionComponent);

				//ѭ������ʵ�����Ϲ��ص������������
				for (auto & Elemelt : entityModel->GetelementsList())
				{
					//�жϵ�ǰʵ���Ƿ�������������ڵ㣩
					if (std::shared_ptr<MissionComponent> missionComponent = std::dynamic_pointer_cast<MissionComponent>(Elemelt))
					{
						missionComponent->AddNode(missionComponentBase);

						return;
					}
				}
			}
		}
	}
}

std::shared_ptr<EntityBase> ObjectManagementMachine::BattleSpaceCreateEntity(const std::shared_ptr<EntityBase>& EntityBasePtr)
{
	if (EntityBasePtr)
	{
		// ���ö���� clone �����õ����������ָ��
		std::shared_ptr<EntityBase> NewEntity = EntityBasePtr->clone();

		//������ָ��ת�������ڵ㣬�������������
		if (std::shared_ptr<EntityModel> NewEntityModel = std::dynamic_pointer_cast<EntityModel>(NewEntity))
		{
			//��ӻ������������̰߳�ȫ����
			std::unique_lock<std::mutex> lock(_mutex);

			//���ʵ�����������������
			NewEntityModel->SetEntityHandle(++EntityHandle);

			this->BattleSpaceVec.push_back(NewEntityModel);
		}

		//�����¼�������ʹ��
		//TODO -> ÿ����һ��ս���ռ�ʵ�壬֪ͨ�¼������� ���л���
		NotifyObserversHandlEntity(NewEntity);

		return NewEntity;
	}
	return nullptr; // ��������ָ��Ϊ�գ��򷵻ؿ�����ָ��
}

void ObjectManagementMachine::AddObserversHandlEntity(std::shared_ptr<EventHandBase> EventHandBasePtr)
{
	this->ObserversList.push_back(EventHandBasePtr);
}

void ObjectManagementMachine::RemoveObserversHandlEntity(std::shared_ptr<EventHandBase> EventHandBasePtr)
{
	this->ObserversList.remove(EventHandBasePtr);
}

void ObjectManagementMachine::NotifyObserversHandlEntity(const std::shared_ptr<EntityBase> EntityBasePtr)
{
	for (auto& element : this->ObserversList)
	{
		element->EventHandEntity(EntityBasePtr);
	}
}
