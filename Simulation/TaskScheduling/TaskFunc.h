#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	任务类.	</summary>
///
/// <remarks>	朱连超, 2024/04/21.	</remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

//struct TaskFunc
/*
struct TaskFunc
{
	TaskFunc(uint64_t expireTime) : _expireTime(expireTime)
	{ }

	std::function<void()> _func;
	int64_t _expireTime = 0;  //超时的绝对时间
};
typedef shared_ptr<TaskFunc> TaskFuncPtr;
*/

class TaskFunc
{
public:
	TaskFunc(uint64_t expireTime);
	~TaskFunc();	

public:

	/// <summary> 没有参数且返回值为 void 的可调用对象 </summary>
	std::function<void()> _func;

	/// <summary> 超时的绝对时间 </summary>
	int64_t _expireTime = 0;
};

typedef std::shared_ptr<TaskFunc> TaskFuncPtr;

