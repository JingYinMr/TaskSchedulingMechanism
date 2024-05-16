#pragma once

//ThreadPool.h
#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <future>
#include <functional>
#include <iostream>
#include <queue>
#include <mutex>
#include <memory>
#include<WinSock2.h>

#include "TaskFunc.h"

#ifdef WIN32
#include <windows.h>
#else
#include <time.h>
#endif

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	跨平台获取当前时间的函数.	</summary>
///
///	在 Windows 平台下，使用 GetLocalTime 函数获取本地时间，并将其转换为 UNIX 时间（以秒为单位），同时将毫秒部分保存到 tv.tv_usec 中。
///	在非 Windows 平台下，调用系统提供的 gettimeofday 函数来获取当前时间。
///
/// <remarks>	朱连超, 2024/04/21.	</remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

int gettimeofday(struct timeval & tv);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	跨平台获取当前时间的函数.	</summary>
///
/// 
///	在 iOS 和 Linux 平台下，根据 _buf_idx 的值从缓冲中获取时间，并根据 _cpu_cycle 的值进行处理。
/// 在其他平台下，调用 gettimeofday 函数来获取当前时间。
///
/// <remarks>	朱连超, 2024/04/21.	</remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

void getNow(struct timeval & tv);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	跨平台获取当前时间的函数.	</summary>
///
///	调用 getNow 函数获取当前时间，并将秒部分乘以 1000，并加上毫秒部分的值，得到以毫秒为单位的时间戳。
///
/// <remarks>	朱连超, 2024/04/21.	</remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

int64_t getNowMs();

#define TNOW getNow()
#define TNOWMS getNowMs()

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	线程池（生产者-消费者模型）.	</summary>
///
///	<notes>
/// 本项目是基于C++11的线程池。使用了许多C++的新特性，包含不限于模板函数泛型编程、
///	std::future、std::packaged_task、std::bind、std::forward完美转发、std::make_shared智能指针、
///	decltype类型推断、std::unique_lock锁等C++11新特性功能。
///	参考：https://developer.aliyun.com/article/1421329 
///	参考：https://blog.csdn.net/baiduwaimai/article/details/128444989
/// </notes>
///
/// <param name="Target">  目标. </param>
/// <returns>	如果,返回true； 否则，返回false. </returns>
///
/// <remarks>	朱连超, 2024/04/21.	</remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class ThreadPool {
public:
	ThreadPool();
	virtual ~ThreadPool();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	设置线程的数量.	</summary>
	///
	///	/*备注：init的功能是初始化线程池，主要是设置线程的数量到类的成员变量中。
	///
	/// <remarks>	朱连超, 2024/04/21.	</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual bool init(size_t num);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	启动线程池，并通过run()执行任务.	</summary>
	///
	///	/*备注：创建线程，并把线程池存入vector中，后面释放线程池时，好一一释放线程。
	///
	/// <remarks>	朱连超, 2024/04/21.	</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual bool start();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	封装任务到任务队列中.	</summary>
	///
	///	1.用线程池启动任务(F是function, Args是参数)
	/// 2.返回任务的future对象，可以通过这个对象来获取返回值
	///
	/// <remarks>	朱连超, 2024/04/21.	</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class F, class... Args>
	auto exec(F&& f, Args&&...args)->std::future<decltype(f(args...))>;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	封装任务到任务队列中.	</summary>
	///
	///	/*备注：exec是将我们的任务存入任务队列中，这段代码是本项目最难的，用了很多C++的新特性。
	///	1.使用了可变参数模板函数。
	///	2.tasks_：保存任务的队列
	///	3.condition_.notify_one()：保存一个任务唤醒一个条件变量
	///	4.std::future : 异步指向某个任务，然后通过future特性去获取任务函数的返回结果。
	///	5.std::bind：将参数列表和函数绑定，生成一个新的可调用对象
	///	6.std::packaged_task：将任务和feature绑定在一起的模板，是一种封装对任务的封装。
	///
	/// <remarks>	朱连超, 2024/04/21.	</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class F, class... Args>
	auto exec(int64_t timeoutMs, F&& f, Args&&... args)->std::future<decltype(f(args...))>;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	等待所有任务执行完成.	</summary>
	///
	///	备注：等待所有任务执行完成。
	///	等待当前任务队列中，所有工作全部结束（队列无任务）
	///	millsecond 等待的时间（ms）， - 1, ：永远等待
	///
	/// <remarks>	朱连超, 2024/04/21.	</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual bool waitForAllDone(int millsecond = -1);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	分离线程，释放内存.	</summary>
	///
	///	/*备注：终止线程池。会调用waitForAllDone等待所有任务执行完成再终止。
	///
	/// <remarks>	朱连超, 2024/04/21.	</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void stop(); 

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	获取线程个数.	</summary>
	///
	/// <remarks>	朱连超, 2024/04/21.	</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	size_t getThreadNum();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	获取当前线程池线的任务数.	</summary>
	///
	/// <remarks>	朱连超, 2024/04/21.	</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	size_t getJobNum();

protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////
   /// <summary>	读取任务队列中的任务.	</summary>
   ///
   ///	/*备注：从任务队列中获取获取任务，这里其实就是我们的消费者模块。
   ///
   /// <remarks>	朱连超, 2024/04/21.	</remarks>
   ////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual bool get(TaskFuncPtr& task);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	通过get()读取任务并执行.	</summary>
	///
	///	/*备注：这里是运行我们的任务部分。包括调用get在任务队列中获取任务，以及执行任务。
	///
	/// <remarks>	朱连超, 2024/04/21.	</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void run();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	线程池是否退出.	</summary>
	///
	/// <remarks>	朱连超, 2024/04/21.	</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool isTerminate() { return _bTerminate; }

private:

	/// <summary>	任务队列.	</summary>
	queue<TaskFuncPtr> _tasks;

	/// <summary>	工作线程.	</summary>
	std::vector<std::thread*> _threads;

	/// <summary>	线程互斥量.	</summary>
	std::mutex _mutex;

	/// <summary>	线程条件变量.	</summary>
	std::condition_variable _condition;

	/// <summary>	线程数.	</summary>
	size_t _threadNum;

	/// <summary>	终止变量.	</summary>
	bool _bTerminate;

	/// <summary>	原子变量.	</summary>
	std::atomic<int> _atomic{ 0 };

};

template<class F, class ...Args>
auto ThreadPool::exec(F && f, Args && ...args) -> std::future<decltype(f(args ...))>
{
	return exec(0, f, args...);
}

/* template <class F, class... Args>
   它是c++里新增的最强大的特性之一，它对参数进行了高度泛化，它能表示0到任意个数、任意类型的参数
   auto exec(F &&f, Args &&... args) -> std::future<decltype(f(args...))>
   std::future<decltype(f(args...))>：返回future，调用者可以通过future获取返回值
   返回值后置 */
template<class F, class ...Args>
inline auto ThreadPool::exec(int64_t timeoutMs, F && f, Args && ...args) -> std::future<decltype(f(args ...))>
{
	// 根据超时时间计算任务的过期时间 `expireTime`，如果超时时间为 0，则任务不会过期。
	int64_t expireTime = (timeoutMs == 0 ? 0 : TNOWMS + timeoutMs);

	//定义返回值类型
	using RetType = decltype(f(args...));

	// 使用 `decltype` 推导出 `f(args...)` 的返回值类型，并将其定义为 `RetType`(这里的using和typedef功能一样，就是为一个类型起一个别名)。
	// 封装任务 使用 `std::packaged_task` 将可调用对象 `f` 和其它参数 `args...` 封装成一个可执行的函数，并将其存储在一个 `std::shared_ptr` 对象 `task` 中。
	auto task = std::make_shared<std::packaged_task<RetType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

	// 封装任务指针，设置过期时间 创建一个 `TaskFunc` 对象，并将任务的过期时间 `expireTime` 传递给它。
	TaskFuncPtr fPtr = std::make_shared<TaskFunc>(expireTime);

	// 具体执行的函数 将封装好的任务函数存储在 `TaskFunc` 对象的 `_func` 成员中，该函数会在任务执行时被调用。
	fPtr->_func = [task]() {
		(*task)();
	};

	std::unique_lock<std::mutex> lock(_mutex);
	_tasks.push(fPtr);              // 将任务插入任务队列中
	_condition.notify_one();        // 唤醒阻塞的线程，可以考虑只有任务队列为空的情况再去notify
	return task->get_future();		//返回一个 `std::future` 对象，该对象可以用于获取任务执行的结果。
}

#endif