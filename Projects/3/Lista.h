#pragma once

//using namespace std;

class Lista
{
public:
	struct Element {
		int value;
		Element* npx = nullptr;
		bool isQueue = false;
	};
	int size = 0;
	int count = 0;

	Element* first = nullptr;
	Element* last = nullptr;

	Element* preFront = nullptr;
	Element* preBack = nullptr;
	Element* front = nullptr;
	Element* back = nullptr;

	Element* XOR(Element* prev, Element* next);
	void AddBeg(int x);
	void AddEnd(int x);
	void DelBeg();
	void DelEnd();
	void PrintForward();
	void PrintBackward();
	void Size();

	void Push2(int x); //-
	void PrintQueue2(); //-
	void Pop2(); //-
	void GarbageSoft();
	void GarbageHard();
	~Lista();
};

