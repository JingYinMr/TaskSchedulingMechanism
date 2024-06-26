#pragma once

#include<iostream>

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	实体基类.	</summary>
///
/// <remarks>	朱连超, 2024/04/30.	</remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class EntityBase
{
public:
	EntityBase();
	virtual ~EntityBase();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	核心接口，多态重写.	</summary>
	///
	/// <remarks>	朱连超, 2024/04/29.	</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void Process() = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	用于实现对象的深拷贝.	</summary>
	///
	/// <remarks>	朱连超, 2024/05/07.	</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual virtual std::shared_ptr<EntityBase> clone() const = 0;

};

