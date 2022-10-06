#include "Kolejka.h"
#include <iostream>

using namespace std;

void Kolejka::Push(int x)
{
	if (!lista->first)
	{
		lista->AddBeg(x);
		front = back = lista->first;
		front->isQueue = true;
	}
	else
	{
		if (!front)
		{
			front = back = lista->first;
			front->value = x;
			front->isQueue = true;
		}
		else if (lista->size == count)
		{
			if (front == lista->first)
			{
				lista->AddBeg(x);
				Element* tmp = front;
				Element* preTmp = pre;
				Element* preCpy;
				while (tmp != back)
				{
					preCpy = tmp;
					tmp = XOR(preTmp, tmp->npx);
					preTmp = preCpy;
				}
				back = XOR(preTmp, tmp->npx);
				if (!back)
					back = lista->first;
				back->value = x;
			}
		}
		else
		{
			Element* tmp = front;
			Element* preTmp = pre;
			Element* preCpy;
			while (tmp != back)
			{
				preCpy = tmp;
				tmp = XOR(preTmp, tmp->npx);
				preTmp = preCpy;
			}
			back = XOR(preTmp, tmp->npx);
			if (!back)
				back = lista->first;
			back->value = x;
		}
	}
	count++;
}
void Kolejka::Pop()
{
	if (front)
	{
		if (front == back)
			front = nullptr;
		else
		{
			if (XOR(pre, front->npx) == NULL)
			{
				pre = nullptr;
				front = lista->first;
			}
			else
			{
				Element* temp = front;
				front = XOR(pre, front->npx);
				pre = temp;
			}
		}
	count--;
	}
}

void Kolejka::Push2(int x)
{
	if (lista->first)
	{
		if (count == lista->size)
		{
			if (back == lista->first)
			{
				lista->AddBeg(x);
				queue.push_back(lista->first);
				pre = back;
				back = lista->first;
			}
			else if(front==lista->first)
			{
				Element* newEl = new Element;
				Element* nextEl = XOR(back->npx,pre); //Next to back el
				newEl->value = x;
				newEl->npx = XOR(nextEl,back);
				nextEl->npx = XOR(XOR(nextEl->npx, back), newEl);
				back->npx = XOR(newEl,pre);
				queue.push_back(newEl);
				pre = back;
				back = newEl;
				lista->size++;
			}
			else
			{
				Element* newEl = new Element;
				newEl->value = x;
				newEl->npx = XOR(front, back);
				back->npx = XOR(newEl, XOR(front, back->npx));
				front->npx = XOR(XOR(front->npx, back), newEl);
				queue.push_back(newEl);
				pre = back;
				back = newEl;
				lista->size++;
			}
		}
		else
		{
			if (front)
			{
				if (back == lista->first)
				{
					pre = nullptr;
					back = lista->last;
					back->value = x;
				}
				else
				{
					Element* next = XOR(back->npx, pre);
					pre = back;
					back = next;
					back->value = x;
					//if(next) //if back is going to be NULL
					//	back = next;
					//else //skip to last element of list
					//{
					//	back = lista->last;
					//	pre = nullptr;
					//}
					//back->value = x;
				}
			}
			else if (back) //if queue was deleted to null (front null, back not null) then front=back
				front = back;
			else
			{
				back = front = lista->first;
				pre = XOR(NULL, back->npx);
			}
			back->value = x;
			queue.push_back(back);
		}
	}
	else
	{
		lista->AddBeg(x);
		queue.push_back(lista->first);
		front = back = lista->first;
	}
	count++;
	back->isQueue = true;
}
void Kolejka::PrintQueue2()
{
	if (queue.empty())
		cout << "NULL" << endl;
	else
	{
		for (Element* n : queue)
			cout << n->value << " ";
		cout << endl;
	}
}
void Kolejka::Pop2()
{
	if (front)
	{
		cout << queue.front()->value << endl;
		queue.front()->isQueue = false;
		queue.pop_front();
		if (queue.empty())
			front = nullptr;
		else
			front = queue.front();
		count--;
	}
	else
		cout << "NULL" << endl;
}
void Kolejka::AddEnd(int x)
{
	if (lista->size && lista->last->isQueue) // because back is equal to lista->last at start of program
	{
		if (back == lista->first)
			lista->AddBeg(x);
		else
		{
			Element* newEl = new Element;
			newEl->value = x;
			Element* prev = nullptr;
			Element* curr = lista->last;
			Element* next = nullptr;
			while (curr != back)
			{
				next = XOR(curr->npx, prev);
				prev = curr;
				curr = next;
			}
			next = XOR(curr->npx, prev);
			newEl->npx = XOR(next, curr);
			curr->npx = XOR(newEl, prev);
			next->npx = XOR(XOR(next->npx, curr), newEl);
			lista->size++;
		}
	}
	else
		lista->AddEnd(x);
}
void Kolejka::AddBeg(int x)
{
	if (lista->size && lista->first->isQueue)
	{
		if (front == lista->last)
		{
			lista->AddEnd(x);
		}
		else
		{
			Element* newEl = new Element;
			newEl->value = x;
			Element* prev = nullptr;
			Element* curr = lista->first;
			Element* next = nullptr;
			while (curr != front)
			{
				next = XOR(prev, curr->npx);
				prev = curr;
				curr = next;
			}
			next = XOR(prev, curr->npx);
			newEl->npx = XOR(curr, next);
			curr->npx = XOR(prev, newEl);
			next->npx = XOR(newEl, XOR(curr, next->npx));
			lista->size++;
		}
	}
	else
		lista->AddBeg(x);
}
void Kolejka::DelBeg()
{
	if (lista->size)
	{
		if (lista->size == 1)
		{
			if (count)
				queue.pop_front();
			back = front = pre = nullptr;
			lista->DelBeg();
			count = 0;
		}
		else if (lista->first->isQueue)
		{
			if (back == lista->first)
			{
				if (count)
				{
					queue.pop_back();
					count--;
					if (count)
					{
						Element* tempPre = pre;
						pre = XOR(back, pre->npx);
						back = tempPre;
					}
					if (!count)
					{
						front = nullptr;
						back = pre; //@#!#!@##!@@#!
						if (lista->size == 2)
							pre = nullptr;
						else
							pre = XOR(back, pre->npx);
						//pre = nullptr;
					}
				}
			}
			else if (front == lista->first)
			{
				queue.pop_front();
				front = queue.front();
				count--;
			}
			else //if deleted el. is not front or back : B|-xxxxxFxxxB|
			{
				queue.remove(lista->first);
				count--;
			}
			lista->DelBeg();
		}
		else //if deleted el is not queue item
		{
			if (back == lista->first) //because back is always pointer either if count=0
			{
				lista->DelBeg();
				back = lista->first; //first or last?
				if (lista->size == 1)
					pre = nullptr;
				else
					pre = XOR(NULL, lista->first->npx);
			}
			else
				lista->DelBeg();
		}
	}
	else
		cout << "NULL" << endl;
}
void Kolejka::DelEnd()
{
	if (lista->size)
	{
		if (lista->last->isQueue)
		{
			if (pre == lista->last)
			{
				queue.remove(lista->last); //can I del pre-last el from list ? maybe size-1?
				count--;
				if (count)
					pre = nullptr;
				else
					front = nullptr; 
				lista->DelEnd();
			}
			else if (back == lista->last)
			{
				queue.pop_back();
				count--;
				if (count)
				{
					pre = XOR(NULL, pre->npx);
					back = lista->first;
				}
				else
				{
					if (lista->size == 1)
						back = front = pre = nullptr;
					else
					{
						pre = XOR(NULL, pre->npx);
						back = lista->first;
						front = nullptr;
					}
					lista->DelEnd();
				}
			}
			else
			{
				queue.remove(lista->last);
				lista->DelEnd();
				count--;
			}
		}
		else
		{
			if (lista->size == 1) //its other situation than above back=front=pre=... because if i Pop and que is empty but 1 item on list then back is not NULL
			{
				back = front = pre = nullptr;
			}
			else if (pre == lista->last)
				pre = nullptr;
			else if (back == lista->last)
			{
				back = lista->first;
				pre = XOR(NULL, lista->first->npx);
			}
			lista->DelEnd();
		}
	}
	else
		cout << "NULL" << endl;
}

void Kolejka::PrintQueue()
{
	if (front)
	{
		Element* temp = front;
		Element* preTemp = pre;
		Element* preCpy = front;
		while (temp!=back)
		{
			preCpy = temp;
			cout << temp->value << " ";
			temp = XOR(preTemp, temp->npx);
			if (!temp)
			{
				temp = lista->first;
				preTemp = nullptr;
			}
			preTemp = preCpy;
		}
		cout << temp->value << endl;
	}
	else
		cout << "NULL" << endl;
}

//Kolejka::~Kolejka()
//{
//	cout << "s";
//	delete lista;
//}