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
	//1.处理当前结点

	//2.处理叶子结点
	for (auto &Elemelt : this->elementsList)
	{
		//虚函数的调用
		Elemelt->Motion();
	}
}
