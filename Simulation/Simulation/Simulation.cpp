// Simulation.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include "ObjectManagementMachine.h"
#include "EntityFactory.h"
#include "ComponentFactory.h"
#include "MissionFactory.h"
#include "MissionComponentBase.h"
#include "MissionComponent.h"
#include "TaskSchedulingMechanism.h"
#include "DrawMilitaryStandards.h"

#include "EntityModel.h"

#define ObjectManagementMachine() ObjectManagementMachine::Instance()
#define TaskSchedulingMechanism() TaskSchedulingMechanism::Instance()

using namespace std;

//创建实体模板和组件模板
void CreateModel()
{
	//单例模式创建对象模板
	std::shared_ptr<EntityFactory> entityFactory = std::make_shared<EntityFactory>();
	for (size_t i = 0; i < 2; i++)
	{
		ObjectManagementMachine()->CreateObject(entityFactory);
	}

	//创建任务模板（侦察任务）
	std::shared_ptr<ReconnaissanceMissionFactory> reconnaissanceMissionFactory = std::make_shared<ReconnaissanceMissionFactory>();
	for (size_t i = 0; i < 100; i++)
	{
		ObjectManagementMachine()->CreateObject(reconnaissanceMissionFactory);
	}
	

	//创建任务模板（打击任务）
	std::shared_ptr<AttackMissionFactory> attackMissionFactory = std::make_shared<AttackMissionFactory>();
	for (size_t i = 0; i < 100; i++)
	{
		ObjectManagementMachine()->CreateObject(attackMissionFactory);
	}
}

//将对象和组件结合
void CreateObject()
{
	// 使用静态成员函数 cloneChild 复制子类对象并返回父类智能指针
	for (auto & elemelt : ObjectManagementMachine()->GetEntityModelVec())
	{
		std::shared_ptr<EntityBase> clonedPtr = ObjectManagementMachine()->cloneEntity(elemelt);
		//clonedPtr->Process();
		

		/*if (clonedPtr != elemelt)
		{
			std::cout << "深拷贝成功" << std::endl;
		}
		else
		{
			std::cout << "深拷贝失败" << std::endl;
		}*/
	}

	//获取实体
	for (auto & elemeltEntity : ObjectManagementMachine()->GetObjectManger())
	{
		//通过抽象类管理实体对象，将抽象实体转换为具体实体
		if (std::shared_ptr<EntityModel> entityModel = std::dynamic_pointer_cast<EntityModel>(elemeltEntity))
		{
			//std::cout << "实体的Handle: " << entityModel->GetEntityHandle() << std::endl;
		}

		//获取组件模板，并复制一份，与实体绑定
		for (auto & elemeltComponent : ObjectManagementMachine()->GetComponentModelVec())
		{
			ObjectManagementMachine()->EntityCombineComponent(elemeltEntity, elemeltComponent);
		}
	}
}

//模拟战场空间
void BattleSpace()
{
	//将对象批量化，同一个类型的组合实体，多个实例
	for (auto & elemelt : ObjectManagementMachine()->GetObjectManger())
	{
		//每一种类型实体创建十个实例
		for (size_t i = 0;i < 10;i++)
		{
			ObjectManagementMachine()->BattleSpaceCreateEntity(elemelt);
		}
	}
}

//事件处理器，添加到对象管理器中，观察对象的创建，每创建一个对象，自动进行处理
void EventHandle()
{
	std::shared_ptr<DrawMilitaryStandards> DrawMilitaryStandardsPtr = std::make_shared<DrawMilitaryStandards>();
	ObjectManagementMachine()->AddObserversHandlEntity(DrawMilitaryStandardsPtr);
}

int main()
{
	//
	EventHandle();

	//
	CreateModel();

	//
	CreateObject();

	//
	BattleSpace();

	//使用任务调度机制
	TaskSchedulingMechanism()->ExecuteMissionForObject();
	
	//获取对象管理器
	/*for (auto & elemeltEntity : ObjectManagementMachine()->GetObjectManger())
	{
		//实体将父类转换为子类
		if (std::shared_ptr<EntityModel> entityModel = std::dynamic_pointer_cast<EntityModel>(elemeltEntity))
		{
			//当前实体句柄
			std::cout << "实体的Handle: " << entityModel->GetEntityHandle() << std::endl;
			entityModel->Process();

			//循环遍历实体身上挂载的所有任务组件
			//for (auto & Elemelt : entityModel->GetelementsList())
			//{
			//	//判断当前实体是否有任务对象（树节点）
			//	if (std::shared_ptr<MissionComponent> missionComponent = std::dynamic_pointer_cast<MissionComponent>(Elemelt))
			//	{
			//		//TODO
			//		missionComponent->PerformTasks();
			//	}
			//}
		}
	}*/
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
