#include "pch.h"
#include<iostream>

#include "DrawMilitaryStandards.h"

using namespace std;

DrawMilitaryStandards::~DrawMilitaryStandards()
{

}

void DrawMilitaryStandards::EventHandEntity(std::shared_ptr<EntityBase>)
{
	std::cout << "���ƾ���" << std::endl;
}
