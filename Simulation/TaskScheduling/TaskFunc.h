#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	������.	</summary>
///
/// <remarks>	������, 2024/04/21.	</remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

//struct TaskFunc
/*
struct TaskFunc
{
	TaskFunc(uint64_t expireTime) : _expireTime(expireTime)
	{ }

	std::function<void()> _func;
	int64_t _expireTime = 0;  //��ʱ�ľ���ʱ��
};
typedef shared_ptr<TaskFunc> TaskFuncPtr;
*/

class TaskFunc
{
public:
	TaskFunc(uint64_t expireTime);
	~TaskFunc();	

public:

	/// <summary> û�в����ҷ���ֵΪ void �Ŀɵ��ö��� </summary>
	std::function<void()> _func;

	/// <summary> ��ʱ�ľ���ʱ�� </summary>
	int64_t _expireTime = 0;
};

typedef std::shared_ptr<TaskFunc> TaskFuncPtr;

