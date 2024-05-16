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

	// �����̶߳��������߳������У�ֱ�ӷ���false
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

	//�ж��Ƿ��д�ִ�е�����
	if (_tasks.empty())
	{
		//�̹߳���
		//��ǰ�߳��ڻ����� lock �ı����µȴ��������� _condition��
		//ֱ�� _bTerminate ����Ϊ��� _tasks ������Ϊ��ʱ�ż���ִ�С�
		_condition.wait(lock, [this] {return _bTerminate || !_tasks.empty(); });
	}

	if (_bTerminate)
	{
		return false;
	}

	if (!_tasks.empty())
	{
		task = std::move(_tasks.front());// ʹ�����ƶ�����
		_tasks.pop();//�ͷ���Դ���ͷ�һ������
		return true;
	}
	return false;
}

void ThreadPool::run()
{
	// ���ô�����
	while (!isTerminate())// �ж��Ƿ���Ҫ�ն�
	{
		TaskFuncPtr task;
		bool ok = get(task); // ��ȡ����
		if (ok)
		{
			++_atomic;
			try
			{
				if (task->_expireTime != 0 && task->_expireTime < TNOWMS)
				{
					// ��ʱ�����Ƿ���Ҫ�����أ�
				}
				else
				{
					task->_func(); // ִ������
				}
			}
			catch (const std::exception&)
			{

			}
			--_atomic;

			// ����ִ�����
			std::unique_lock<std::mutex> lock(_mutex);
			if (_atomic == 0 && _tasks.empty())
			{
				_condition.notify_all(); //  ֪ͨwaitForAllDone
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
		//����߳��ǿɼ���ģ�����δ�� join��������� join �����ȴ��߳̽�����
		if (_threads[i]->joinable())
		{
			_threads[i]->join();
		}

		//Ȼ���ͷŸ��̵߳���Դ��ͨ�� delete _threads[i] ���ͷ��̶߳�����ڴ档
		//�� _threads[i] ��Ϊ nullptr����ȷ����ָ�벻��ָ���κ����ݡ�
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
