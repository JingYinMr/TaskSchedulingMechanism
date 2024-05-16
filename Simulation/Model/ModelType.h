#pragma once

/// <summary> 实体类型 </summary>
enum EntityType
{
	/// <summary> 未知类别  </summary>
	ET_Unknown = 0,

	/// <summary> 飞机  </summary>
	ET_Airplane = 1,

	/// <summary> 轮船  </summary>
	ET_Ship = 2,

};

/// <summary> 组件类型 </summary>
enum ComponentType
{
	/// <summary> 未知类别  </summary>
	CT_Unknown = 0,

	/// <summary> 机动组件  </summary>
	CT_Motion = 1,

	/// <summary> 任务组件  </summary>
	CT_Mission = 2,

	/// <summary> 探测组件  </summary>
	CT_Deterct = 3,

};