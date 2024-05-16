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
	static ObjectManagementMachine object;//静态对象
	return &object;//返回对象的入口地址
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
	//想要创建任务对象
	if (std::shared_ptr<MissionComponentFactory> missionComponentFactory = std::dynamic_pointer_cast<MissionComponentFactory>(ComponentFactory))
	{
		//如果组件管理器中没有任务组件，创建一个任务组件对象，用来管理具体任务
		if (!IsMissionComponent)
		{
			//创建任务组件
			std::shared_ptr<MissionComponentFactory> missionComponentFactory = std::make_shared<MissionComponentFactory>();
			CreateComponent createComponent(missionComponentFactory);
			std::shared_ptr<ComponentBase> missionComponent = createComponent.CreateComponentFactory();

			//放入组件模板管理器 用于管理所有任务组件
			this->ComponentModelVec.push_back(missionComponent);

			IsMissionComponent = true;
		}
	}

	for (auto & elemelt : this->ComponentModelVec)
	{
		//获取任务组件
		if (std::shared_ptr<MissionComponent> missionComponent = std::dynamic_pointer_cast<MissionComponent>(elemelt))
		{
			CreateComponent createComponent(ComponentFactory);
			std::shared_ptr<ComponentBase> Mission = createComponent.CreateComponentFactory();
				
			//将具体任务，放入任务组件中
			missionComponent->AddNode(std::dynamic_pointer_cast<MissionComponentBase>(Mission));

			return;
		}
	}
}

std::shared_ptr<EntityBase> ObjectManagementMachine::cloneEntity(const std::shared_ptr<EntityBase>& EntityBasePtr)
{
	if (EntityBasePtr) 
	{
		// 调用对象的 clone 方法得到对象的智能指针
		std::shared_ptr<EntityBase> NewEntity = EntityBasePtr->clone();

		this->EntityVec.push_back(NewEntity);

		return NewEntity;
	}
	return nullptr; // 如果传入的指针为空，则返回空智能指针
}

std::shared_ptr<ComponentBase> ObjectManagementMachine::cloneComponent(const std::shared_ptr<ComponentBase>& ComponentBasePtr)
{
	if (ComponentBasePtr)
	{
		// 调用对象的 clone 方法得到对象的智能指针
		std::shared_ptr<ComponentBase> NewComponent = ComponentBasePtr->clone();

		return NewComponent;
	}
	return nullptr; // 如果传入的指针为空，则返回空智能指针
}

void ObjectManagementMachine::EntityCombineComponent(std::shared_ptr<EntityBase>& EntityBasePtr, const std::shared_ptr<ComponentBase>& ComponentBasePtr)
{
	std::shared_ptr<ComponentBase> NewComponent = cloneComponent(ComponentBasePtr);

	//判断该结点是否为任务结点
	if (std::shared_ptr<MissionComponentBase> missionComponentBase = std::dynamic_pointer_cast<MissionComponentBase>(NewComponent))
	{
		//实体将父类转换为子类
		if (std::shared_ptr<EntityModel> entityModel = std::dynamic_pointer_cast<EntityModel>(EntityBasePtr))
		{
			//循环遍历实体身上挂载的所有任务组件
			for (auto & Elemelt : entityModel->GetelementsList())
			{
				//判断当前实体是否有任务对象（树节点）
				if (std::shared_ptr<MissionComponent> missionComponent = std::dynamic_pointer_cast<MissionComponent>(Elemelt))
				{
					missionComponent->AddNode(missionComponentBase);

					return;
				}
			}

			//没有任务组件，就创建一个任务组件对象
			{
				//创建任务组件
				std::shared_ptr<MissionComponentFactory> missionComponentFactory = std::make_shared<MissionComponentFactory>();
				CreateComponent createComponent(missionComponentFactory);
				std::shared_ptr<ComponentBase> missionComponent = createComponent.CreateComponentFactory();

				//放入组件模板管理器 用于管理所有任务组件
				entityModel->AddNode(missionComponent);

				//循环遍历实体身上挂载的所有任务组件
				for (auto & Elemelt : entityModel->GetelementsList())
				{
					//判断当前实体是否有任务对象（树节点）
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
		// 调用对象的 clone 方法得到对象的智能指针
		std::shared_ptr<EntityBase> NewEntity = EntityBasePtr->clone();

		//将父类指针转换成树节点，用于添加组件结点
		if (std::shared_ptr<EntityModel> NewEntityModel = std::dynamic_pointer_cast<EntityModel>(NewEntity))
		{
			//添加互斥锁，考虑线程安全问题
			std::unique_lock<std::mutex> lock(_mutex);

			//添加实体句柄（句柄管理器）
			NewEntityModel->SetEntityHandle(++EntityHandle);

			this->BattleSpaceVec.push_back(NewEntityModel);
		}

		//搭配事件处理器使用
		//TODO -> 每创建一个战场空间实体，通知事件处理器 进行绘制
		NotifyObserversHandlEntity(NewEntity);

		return NewEntity;
	}
	return nullptr; // 如果传入的指针为空，则返回空智能指针
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
