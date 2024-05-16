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
	static TaskSchedulingMechanism object;//��̬����
	return &object;//���ض������ڵ�ַ
}

void TaskSchedulingMechanism::ExecuteMissionForObject()
{
	ThreadPool threadpool;
	threadpool.init(4); // ��ʼ���̳߳�����
	threadpool.start(); // �����̳߳�

	//���������������ִ������
	for (auto& elemelt : ObjectManagementMachine()->GetBattleSpaceVec())
	{
		// ʹ�� std::bind ����Ա����ָ��󶨵������ϣ������󶨺����������̳߳�
		//auto f1 = threadpool.exec(std::bind(&EntityBase::Process, elemelt));

		// ʹ�� lambda ���ʽ���� elemelt��������������̳߳�
		auto f1 = threadpool.exec([elemelt]() {
			elemelt->Process();
		});
	}

	threadpool.waitForAllDone();
	threadpool.stop();
}
