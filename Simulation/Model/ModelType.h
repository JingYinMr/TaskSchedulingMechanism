#pragma once

/// <summary> ʵ������ </summary>
enum EntityType
{
	/// <summary> δ֪���  </summary>
	ET_Unknown = 0,

	/// <summary> �ɻ�  </summary>
	ET_Airplane = 1,

	/// <summary> �ִ�  </summary>
	ET_Ship = 2,

};

/// <summary> ������� </summary>
enum ComponentType
{
	/// <summary> δ֪���  </summary>
	CT_Unknown = 0,

	/// <summary> �������  </summary>
	CT_Motion = 1,

	/// <summary> �������  </summary>
	CT_Mission = 2,

	/// <summary> ̽�����  </summary>
	CT_Deterct = 3,

};