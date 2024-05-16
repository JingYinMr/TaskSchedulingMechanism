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

//std::function<void()> func的用法
/*
void foo() 
{
	//TODO
}

int main() 
{
	std::function<void()> func;

	// 将函数指针赋值给 func
	func = &foo;
	func(); // 调用 foo 函数
}
*/
