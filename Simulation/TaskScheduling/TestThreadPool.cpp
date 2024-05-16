#include "pch.h"

#include<iostream>
#include <thread>
#include <chrono>

#include "TestThreadPool.h"

using namespace std;

/// <summary> �ӳ� </summary>
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
	ThreadPool threadpool;     // ��װһ���̳߳�
	threadpool.init(5);             // �����̵߳�����
	threadpool.start();              // �����̳߳أ������̣߳� �߳�û��start��������Ϻ󱻵���

	// ����Ҫִ�е�����
	// threadpool.exec(1000,func0); // 1000�ǳ�ʱ1000����˼��Ŀǰû��������

	auto test = [](int a) {
		cout << "test =" << a << endl;
	};
	threadpool.exec(test, 10);

	/*���⣺�������
	std::cout �̲߳���ȫ���⣬Ŀǰ���뷨���Ƕ� std::cout �����̰߳�ȫ�ķ�װ����ʹ�û���������ͬ��������
	���������
	1.ʹ���ַ������з�װ
	2.��std::cout��printf�滻
	3.��������ר�Ŷ���һ������࣬������������ͨ�����۲���ģʽ����ϡ�����ģʽ��
	*/
	//ִ��ѭ������
	static int MissionNum = 100;
	while (MissionNum-- >= 0)
	{
		//threadpool.exec(func0, 10);// ��������
		threadpool.exec(std::bind(&TestThreadPool::func0, this));

		// threadpool.exec(func1, 10);// ��������
		threadpool.exec(std::bind(&TestThreadPool::func1, this, std::placeholders::_1), 10);

		//threadpool.exec(func2, 20, "darren"); // ��������
		threadpool.exec(std::bind(&TestThreadPool::func2, this, std::placeholders::_1, std::placeholders::_2), 20, "darren");

		//�ӳ�1��
		delay(1);
	}

	threadpool.waitForAllDone(); // �ȴ���ִ�����˳� ���к��� ����1000������ ��1000������ִ����ϲ��˳�
	threadpool.stop();      // �����������ִ���˳�
}

void TestThreadPool::test2()
{
	ThreadPool threadpool;
	threadpool.init(1);
	threadpool.start(); // �����̳߳�

	// ����Ҫִ�е�����
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
	threadpool.start(); // �����̳߳�

	Test t1;
	t1.setName("Test1");
	Test t2;
	t2.setName("Test2");
	Test t3;
	t3.setName("Test3");

	//��������������
	auto f1 = threadpool.exec(std::bind(&Test::test, &t1, std::placeholders::_1), 10);
	//auto f2 = threadpool.exec(std::bind(&Test::test, &t2, std::placeholders::_1), 20);
	//auto f3 = threadpool.exec(std::bind(&Test::test, &t3, std::placeholders::_1), 30);
	
	/*//ִ������
		f1 �������� std::future<decltype(&Test::test)>����Ϊ threadpool.exec ����һ�� std::future ����
	���а����� std::bind(&Test::test, &t1, std::placeholders::_1) �ķ������ͣ����󶨵� t1 �� Test::test �����ķ������͡�
		���� Test::test �ķ��������� void����ô f1.get() ����������ǰ�߳�ֱ�� Test::test ����ִ����ϣ�
	�������䷵��ֵ������ Test::test ���� void�������ڵ��� f1.get() ֮�󣬲��᷵���κ�ֵ��
	��ֻ�ǵȴ� Test::test ������ִ����ɡ�*/
	cout << "t1 " << f1.get() << endl;
	/*cout << "t2 " << f2.get() << endl;
	cout << "t3 " << f3.get() << endl;*/

	//std::cout << "��ǰ�̳߳�����" << threadpool.getThreadNum() << std::endl;
	//std::cout << "��ǰ��������" << threadpool.getJobNum() << std::endl;

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