#include "pch.h"

#include<iostream>

#include "TaskSchedulingMechanism.h"
#include "ObjectManagementMachine.h"
#include "EntityBase.h"

#include "ThreadPool.h"

#define ObjectManagementMachine() ObjectManagementMachine::Instance()

using namespace std;

TaskSchedulingMechanism::~TaskSchedulingMechanism()
{
}

TaskSchedulingMechanism * TaskSchedulingMechanism::Instance()
{
	static TaskSchedulingMechanism object;//静态对象
	return &object;//返回对象的入口地址
}

void TaskSchedulingMechanism::ExecuteMissionForObject()
{
	ThreadPool threadpool;
	threadpool.init(4); // 初始化线程池数量
	threadpool.start(); // 启动线程池

	//遍历对象管理器，执行任务
	for (auto& elemelt : ObjectManagementMachine()->GetBattleSpaceVec())
	{
		// 使用 std::bind 将成员函数指针绑定到对象上，并将绑定后的任务加入线程池
		//auto f1 = threadpool.exec(std::bind(&EntityBase::Process, elemelt));

		// 使用 lambda 表达式捕获 elemelt，并将任务加入线程池
		auto f1 = threadpool.exec([elemelt]() {
			elemelt->Process();
		});
	}

	threadpool.waitForAllDone();
	threadpool.stop();
}
