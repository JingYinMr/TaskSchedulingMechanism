#pragma once

#include<iostream>

#include "ComponentFactory.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	�������.	</summary>
///
/// <remarks>	������, 2024/05/02.	</remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class CreateComponent
{
public:
	CreateComponent(std::shared_ptr<ComponentFactoryBase> factory);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	ͨ����̬����������ʵ�����.	</summary>
	///
	/// <remarks>	������, 2024/05/02.	</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	std::shared_ptr<ComponentBase> CreateComponentFactory();

private:
	std::shared_ptr<ComponentFactoryBase> Factory;
};
