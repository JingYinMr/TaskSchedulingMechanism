#include "pch.h"

#include<iostream>
#include <thread>
#include <chrono>

#include "TestThreadPool.h"

using namespace std;

/// <summary> 延迟 </summary>
static void delay(int seconds) {
	std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

class Test
{
public:
	int test(int i) {
		cout << getName() << ", i = " << i << endl;
		return i;
	}
	
	void setName(string name) {
		_name = name;
	}

	string getName() {
		return _name;
	}

private:
	string _name;
};

void TestThreadPool::test1()
{
	ThreadPool threadpool;     // 封装一个线程池
	threadpool.init(5);             // 设置线程的数量
	threadpool.start();              // 启动线程池，创建线程， 线程没有start，创建完毕后被调度

	// 假如要执行的任务
	// threadpool.exec(1000,func0); // 1000是超时1000的意思，目前没有起作用

	auto test = [](int a) {
		cout << "test =" << a << endl;
	};
	threadpool.exec(test, 10);

	/*问题：乱序输出
	std::cout 线程不安全问题，目前的想法就是对 std::cout 进行线程安全的封装，并使用互斥锁进行同步操作。
	解决方法：
	1.使用字符串进行封装
	2.将std::cout用printf替换
	3.互斥锁：专门定义一个输出类，将该类上锁，通过“观察者模式”结合“策略模式”
	*/
	//执行循环任务
	static int MissionNum = 100;
	while (MissionNum-- >= 0)
	{
		//threadpool.exec(func0, 10);// 插入任务
		threadpool.exec(std::bind(&TestThreadPool::func0, this));

		// threadpool.exec(func1, 10);// 插入任务
		threadpool.exec(std::bind(&TestThreadPool::func1, this, std::placeholders::_1), 10);

		//threadpool.exec(func2, 20, "darren"); // 插入任务
		threadpool.exec(std::bind(&TestThreadPool::func2, this, std::placeholders::_1, std::placeholders::_2), 20, "darren");

		//延迟1秒
		delay(1);
	}

	threadpool.waitForAllDone(); // 等待都执行完退出 运行函数 插入1000个任务， 等1000个任务执行完毕才退出
	threadpool.stop();      // 这里才是真正执行退出
}

void TestThreadPool::test2()
{
	ThreadPool threadpool;
	threadpool.init(1);
	threadpool.start(); // 启动线程池

	// 假如要执行的任务
	//std::future<decltype (func1_future(0))> result1 = threadpool.exec(func1_future, 10);
	std::future<decltype (func1_future(0))> result1 = threadpool.exec(std::bind(&TestThreadPool::func1_future, this, std::placeholders::_1), 10);

	// std::future<string> result2 = threadpool.exec(func2_future, 20, "darren");
	std::future<string> result2 = threadpool.exec(std::bind(&TestThreadPool::func2_future, this, std::placeholders::_1, std::placeholders::_2), 20, "darren");

	// auto result2 = threadpool.exec(func2_future, 20, "darren");

	std::cout << "result1: " << result1.get() << std::endl;
	std::cout << "result2: " << result2.get() << std::endl;

	threadpool.waitForAllDone();
	threadpool.stop();
}

void TestThreadPool::test3()
{
	ThreadPool threadpool;
	threadpool.init(1);
	threadpool.start(); // 启动线程池

	Test t1;
	t1.setName("Test1");
	Test t2;
	t2.setName("Test2");
	Test t3;
	t3.setName("Test3");

	//任务加入任务队列
	auto f1 = threadpool.exec(std::bind(&Test::test, &t1, std::placeholders::_1), 10);
	//auto f2 = threadpool.exec(std::bind(&Test::test, &t2, std::placeholders::_1), 20);
	//auto f3 = threadpool.exec(std::bind(&Test::test, &t3, std::placeholders::_1), 30);
	
	/*//执行任务
		f1 的类型是 std::future<decltype(&Test::test)>，因为 threadpool.exec 返回一个 std::future 对象，
	其中包含了 std::bind(&Test::test, &t1, std::placeholders::_1) 的返回类型，即绑定到 t1 的 Test::test 函数的返回类型。
		假设 Test::test 的返回类型是 void，那么 f1.get() 将会阻塞当前线程直到 Test::test 函数执行完毕，
	并返回其返回值。由于 Test::test 返回 void，所以在调用 f1.get() 之后，不会返回任何值，
	而只是等待 Test::test 函数的执行完成。*/
	cout << "t1 " << f1.get() << endl;
	/*cout << "t2 " << f2.get() << endl;
	cout << "t3 " << f3.get() << endl;*/

	//std::cout << "当前线程池数量" << threadpool.getThreadNum() << std::endl;
	//std::cout << "当前任务数量" << threadpool.getJobNum() << std::endl;

	threadpool.waitForAllDone();
	threadpool.stop();
}

void TestThreadPool::func0()
{
	cout << "func0()" << endl;
}

void TestThreadPool::func1(int a)
{
	cout << "func1 int =" << a << endl;
}

void TestThreadPool::func2(int a, std::string b)
{
	cout << "func2() a=" << a << ", b=" << b << endl;
}

int TestThreadPool::func1_future(int a)
{
	cout << "func1() a=" << a << endl;
	return a;
}

string TestThreadPool::func2_future(int a, string b)
{
	cout << "func2() a=" << a << ", b=" << b << endl;
	return b;
}