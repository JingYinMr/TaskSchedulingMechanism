#include "pch.h"
#include "ThreadPool.h"

ThreadPool::ThreadPool()
{
}

ThreadPool::~ThreadPool()
{
}

bool ThreadPool::init(size_t num)
{
	std::unique_lock<std::mutex> lock(_mutex);

	// 假如线程队列仍有线程在运行，直接返回false
	if (!_threads.empty())
	{
		return false;
	}

	_threadNum = num;
	return true;
}

bool ThreadPool::get(TaskFuncPtr& task)
{
	std::unique_lock<std::mutex> lock(_mutex);

	//判断是否有待执行的任务
	if (_tasks.empty())
	{
		//线程挂起
		//当前线程在互斥锁 lock 的保护下等待条件变量 _condition，
		//直到 _bTerminate 变量为真或 _tasks 容器不为空时才继续执行。
		_condition.wait(lock, [this] {return _bTerminate || !_tasks.empty(); });
	}

	if (_bTerminate)
	{
		return false;
	}

	if (!_tasks.empty())
	{
		task = std::move(_tasks.front());// 使用了移动语义
		_tasks.pop();//释放资源，释放一个任务
		return true;
	}
	return false;
}

void ThreadPool::run()
{
	// 调用处理部分
	while (!isTerminate())// 判断是否需要终断
	{
		TaskFuncPtr task;
		bool ok = get(task); // 读取任务
		if (ok)
		{
			++_atomic;
			try
			{
				if (task->_expireTime != 0 && task->_expireTime < TNOWMS)
				{
					// 超时任务是否需要处理呢？
				}
				else
				{
					task->_func(); // 执行任务
				}
			}
			catch (const std::exception&)
			{

			}
			--_atomic;

			// 任务执行完成
			std::unique_lock<std::mutex> lock(_mutex);
			if (_atomic == 0 && _tasks.empty())
			{
				_condition.notify_all(); //  通知waitForAllDone
			}
		}
	}
}

bool ThreadPool::start()
{
	std::unique_lock<std::mutex> lock(_mutex);

	if (!_threads.empty())
	{
		return false;
	}

	for (size_t i = 0; i < _threadNum; i++)
	{
		_threads.push_back(new thread(&ThreadPool::run, this));
	}
	return true;

}

bool ThreadPool::waitForAllDone(int millsecond)
{
	std::unique_lock<std::mutex> lock(_mutex);

	if (_tasks.empty() && _atomic == 0) return true;

	if (millsecond < 0)
	{
		_condition.wait(lock, [this] { return _tasks.empty(); });
		return true;
	}
	else
	{
		return _condition.wait_for(lock, std::chrono::milliseconds(millsecond), [this] {return _tasks.empty(); });
	}
}

void ThreadPool::stop()
{
	{
		std::unique_lock<std::mutex> lock(_mutex);

		_bTerminate = true;

		_condition.notify_all();
	}

	for (size_t i = 0; i < _threads.size(); i++)
	{
		//如果线程是可加入的（即尚未被 join），则调用 join 函数等待线程结束。
		if (_threads[i]->joinable())
		{
			_threads[i]->join();
		}

		//然后释放该线程的资源，通过 delete _threads[i] 来释放线程对象的内存。
		//将 _threads[i] 置为 nullptr，以确保该指针不再指向任何内容。
		delete _threads[i];
		_threads[i] = nullptr;
	}

	std::unique_lock<std::mutex> lock(_mutex);
	_threads.clear();
}

size_t ThreadPool::getThreadNum()
{
	std::unique_lock<std::mutex> lock(_mutex);
	return _threads.size();
}

size_t ThreadPool::getJobNum()
{
	std::unique_lock<std::mutex> lock(_mutex);
	return _tasks.size();
}

int gettimeofday(struct timeval & tv)
{
#if WIN32
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tv.tv_sec = clock;
	tv.tv_usec = wtm.wMilliseconds * 1000;
	return 0;
#else
	return ::gettimeofday(tv);
#endif
}

void getNow(timeval & tv)
{
#if TARGET_PLATFORM_IOS || TARGET_PLATFORM_LINUX
	int idx = _buf_idx;
	*tv = _t[idx];
	if (fabs(_cpu_cycle - 0) < 0.0001 && _use_tsc)
	{
		addTimeOffset(*tv, idx);
	}
	else
	{
		TC_Common::gettimeofday(*tv);
	}
#else
	gettimeofday(tv);
#endif
}

int64_t getNowMs()
{
	struct timeval tv;
	getNow(tv);
	return tv.tv_sec * (int64_t)1000 + tv.tv_usec / 1000;
}
