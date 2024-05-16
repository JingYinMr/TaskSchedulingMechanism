#include "pch.h"
#include "MotionComponent.h"

MotionComponent::~MotionComponent()
{
}

void MotionComponent::AddNode(std::shared_ptr<MotionComponentBase> elements)
{
	elementsList.push_back(elements);
}

void MotionComponent::RemoveNode(std::shared_ptr<MotionComponentBase> elements)
{
	elementsList.remove(elements);
}

void MotionComponent::Motion()
{
	//1.����ǰ���

	//2.����Ҷ�ӽ��
	for (auto &Elemelt : this->elementsList)
	{
		//�麯���ĵ���
		Elemelt->Motion();
	}
}
