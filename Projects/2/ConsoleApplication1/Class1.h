#pragma once
//#include <iostream>

class Class1 {
public:
	struct Data;
	struct Node;
	struct Func;
	struct Data* XOR(struct Data* prev, struct Data* next);

	Class1();

	void Actual(Node* actual);

	void Next(Node*& actual, Node* fl, bool print);

	void Prev(Node*& actual, Node*& fl, bool print);

	void PushEnd(Node*& fl, Node*& actual, int num);

	void PushBeg(Node*& fl, Node*& actual, int num);

	void DelBeg(Node*& fl, Node*& actual);

	void DelEnd(Node*& fl, Node*& actual);

	void DelAct(Node*& fl, Node*& actual);

	void DelVal(Node*& fl, Node*& actual, int n);

	void AddAct(Node*& fl, Node*& actual, int n);

	void PrintBackward(Node* fl);

	void PrintForward(Node* fl);

	void DeleteAll(Node* fl, Node* actual);

	void resize(Func*& x, int& n);

	void Functions(Func* x, int n, Node*& fl, Node*& actual);

	
};
