#pragma once
#include "Lista.h"
#include <list>

using namespace std;

class Kolejka : Lista
{
public:
	Lista* lista = new Lista;
	//Lista* queue = new Lista;
	list<Element*> queue;
	void Push(int x);
	void Push2(int x); //-
	void PrintQueue2(); //-
	void Pop2(); //-
	void AddEnd(int x); //-
	void AddBeg(int x);
	void DelEnd();
	void DelBeg();
	
	void PrintQueue();	
	void Pop();
	//~Kolejka();
};
