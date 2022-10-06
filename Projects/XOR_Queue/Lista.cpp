#include "Lista.h"
#include <iostream>

using namespace std;


Lista::Element* Lista::XOR(Element* prev, Element* next)
{
	return (Element*)((uintptr_t)(prev) ^ (uintptr_t)(next));
}

void Lista::AddBeg(int x)
{
	Element* newEl = new Element;
	newEl->value = x;
	if (first == NULL)
	{
		first = last = back = newEl;
	}
	else
	{
		Element* next = XOR(NULL, first->npx);
		newEl->npx = XOR(NULL, first);
		first->npx = XOR(newEl, next);
		if (first->isQueue && first != back)
		{
			newEl->isQueue = true;
			count++;
		}
		first = newEl;
	}
	size++;
}
void Lista::AddEnd(int x)
{
	Element* newEl = new Element;
	newEl->value = x;
	if (first == NULL)
	{
		first = last = back = newEl;
	}
	else
	{
		Element* next = XOR(last->npx, NULL);
		newEl->npx = XOR(last, NULL);
		last->npx = XOR(next, newEl);
		if (size == 1)
			preBack = preFront = newEl;
		if (last->isQueue)
		{
			if (last == back)
			{
				if (count > 1)
				{
					newEl->isQueue = true;
					count++;
				}
				preBack = newEl;
			}
			else if (front == last)
				preFront = newEl;
			else
			{
				count++; newEl->isQueue = true;
			}
		}
		last = newEl;
	}
	size++;
}

void Lista::DelBeg()
{
	if (first)
	{
		if (first == last)
		{
			if (first->isQueue)
			{
				front = nullptr;
				count--;
			}
			delete first;
			last = nullptr;
			first = nullptr;
		}
		else
		{
			Element* next = XOR(NULL, first->npx);
			Element* nextNext = XOR(first, next->npx);
			//next->npx = XOR(first, next->npx); in XOR project should be correct IMO
			if (first->isQueue)
			{
				if (back == first)
				{
					preBack = nextNext;
					if (front == back)
					{
						front = nullptr;
						preFront = preBack;
					}
					back = next;
				}
				else
				{
					if (front == first)
					{
						preFront = nullptr;
						front = last;
					}
				}
				count--;
			}
			else //if back is not null [because we set if back != null and front = null, que is empty
				if (back == first)
				{
					back = next;
					preBack = XOR(first, next->npx);
				}
			next->npx = XOR(NULL, nextNext);
			delete first;
			first = next;
		}
		size--;
	}
	else
		cout << "NULL" << endl;
}
void Lista::DelEnd()
{
	if (first)
	{
		if (first == last)
		{
			if (first->isQueue)
			{
				front = nullptr;
				count--;
			}
			delete first;
			last = nullptr;
			first = nullptr;
		}
		else
		{
			Element* next = XOR(last->npx, NULL);
			Element* nextNext = XOR(next->npx, last);
			if (last->isQueue)
			{
				if (back == last)
				{
					back = first; //when del last back goes to right
					preBack = XOR(NULL, back->npx);
					if (front == last)
					{
						front = nullptr;
						preFront = preBack;
					}
				}
				else
				{
					if (preBack == last)
						preBack = nullptr;
					if (preFront == last)
						preFront = nullptr;
				}
				count--;
			}
			else
			{
				if (back == last)
					back = next;
			}
			next->npx = XOR(nextNext, NULL);
			delete last;
			last = next;
		}
		size--;
	}
	else
		cout << "NULL" << endl;
}

void Lista::PrintForward()
{
	if (first)
	{
		Element* curr = first;
		Element* prev = NULL;
		Element* prev2;
		while (curr)
		{
			cout << curr->value << " ";
			prev2 = curr;
			curr = XOR(prev, curr->npx);
			prev = prev2;
		}
		cout << endl;
	}
	else
		cout << "NULL" << endl;
}
void Lista::PrintBackward()
{
	if (first)
	{
		Element* curr = last;
		Element* prev = NULL;
		Element* prev2;
		while (curr)
		{
			cout << curr->value << " ";
			prev2 = curr;
			curr = XOR(curr->npx, prev);
			prev = prev2;
		}
		cout << endl;
	}
	else
		cout << "NULL" << endl;
}

void Lista::Size()
{
	cout << size << endl;
}

void Lista::Push2(int x)
{
	if (size)
	{
		if (count)
		{
			if (count == size)
			{
				Element* newEl = new Element;
				Element* next = XOR(back->npx, preBack);
				newEl->npx = XOR(next, back);
				if (next)
					next->npx = XOR(XOR(next->npx, back), newEl);
				back->npx = XOR(newEl, preBack);
				if (first == back)
					first = newEl;
				else
					preFront = newEl;
				preBack = back;
				back = newEl;
				size++;
			}
			else
			{
				Element* next = XOR(back->npx, preBack);
				if (next)
				{
					preBack = back;
					back = next;
				}
				else
				{
					back = last;
					preBack = nullptr;
				}
			}
		}
		else
		{
			front = back;
			preFront = preBack;
		}
		back->value = x;
	}
	else
	{
		AddBeg(x);
		front = back;
		//back = front = first;
	}
	count++;
	back->isQueue = true;
}
void Lista::PrintQueue2()
{
	if (!count)
		cout << "NULL" << endl;
	else
	{
		Element* pre = preFront;
		Element* curr = front;
		Element* next;
		while (curr != back)
		{
			cout << curr->value << " ";
			next = XOR(curr->npx, pre);
			pre = curr;
			curr = next;
			if (curr == NULL)
			{
				curr = last;
				pre = nullptr;
			}
		}
		cout << curr->value << " ";
		cout << endl;
	}
}
void Lista::Pop2()
{
	if (front)
	{
		cout << front->value << endl;
		front->isQueue = false;
		if (count==1)
			front = nullptr;
		else
		{
			Element* next = XOR(front->npx, preFront);
			if (next)
			{
				preFront = front;
				front = next;
			}
			else
			{
				front = last;
				preFront = nullptr;
			}
		}
		count--;
	}
	else
		cout << "NULL" << endl;
}
void Lista::GarbageSoft()
{
	if (first)
	{
		Element* curr = first;
		Element* prev = NULL;
		Element* next;
		while (curr)
		{
			if (curr->isQueue == false)
				curr->value = 0;
			next = XOR(prev, curr->npx);
			prev = curr;
			curr = next;
		}
	}
}
void Lista::GarbageHard()
{
	if (first)
	{
		Element* curr = first;
		Element* prev = NULL;
		Element* next;
		while (curr)
		{
			next = XOR(prev, curr->npx);
			if (curr->isQueue == false)
			{
				if(prev)
					prev->npx=XOR(XOR(prev->npx, curr),next);
				if (next)
					next->npx = XOR(prev, XOR(curr, next->npx));
				if (curr == preBack)
					preBack = next;
				if (curr == preFront)
					preFront = next;
				delete curr;
				curr = next;
				size--;
			}
			if (curr != next)
			{
				prev = curr;
				curr = next;
			}
		}
		if (size == 0)
			first = last = front = back = preFront = preBack = nullptr;
	}
}


Lista::~Lista()
{
	//cout << "DESTRUKTOR!!!" << endl;
	if (first)
	{
		Element* curr = first;
		Element* next = XOR(NULL,curr->npx);
		Element* tmp;
		while (curr)
		{
			tmp = curr;
			delete curr;
			curr = next;
			if(next)
				next = XOR(tmp, curr->npx);
		}
		cout << endl;
	}
}