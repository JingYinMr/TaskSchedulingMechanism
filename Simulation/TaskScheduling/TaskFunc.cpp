#include "pch.h"

#include<iostream>
#include <functional>

#include "TaskFunc.h"

TaskFunc::TaskFunc(uint64_t expireTime)
	:_expireTime(expireTime)
{
}

TaskFunc::~TaskFunc()
{
}

//std::function<void()> func���÷�
/*
void foo() 
{
	//TODO
}

int main() 
{
	std::function<void()> func;

	// ������ָ�븳ֵ�� func
	func = &foo;
	func(); // ���� foo ����
}
*/
