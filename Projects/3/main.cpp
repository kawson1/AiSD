#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Lista.h"
#include <time.h>

#define MAX_COMMAND_LENGTH 15
#define SIZE 1000
using namespace std;

struct Func {
	int val;
	int choice;
};

void Functions(Lista* q, Func* func, int n)
{
	for (int j = 0; j < n; j++)
	{
		if (func[j].choice == 0)
			q->Push2(func[j].val); if (func[j].choice == 1)
			q->Pop2(); if (func[j].choice == 2)
			q->PrintQueue2(); if (func[j].choice == 3)
			q->PrintForward(); (func[j].val); if (func[j].choice == 4)
			q->PrintBackward(); if (func[j].choice == 5)
			cout << q->count << endl; if (func[j].choice == 6)
			cout << q->size << endl; if (func[j].choice == 7)
			q->AddBeg(func[j].val); if (func[j].choice == 8)
			q->AddEnd(func[j].val);if (func[j].choice == 9)
			q->DelBeg();if (func[j].choice == 10)
			q->DelEnd();if (func[j].choice == 11)
			q->GarbageSoft();if (func[j].choice == 12)
			q->GarbageHard();
	}
}
void resize(Func*& x, int& n)
{
	int tmp_s = n * 2;
	Func* tmp = new Func[tmp_s];
	for (int i = 0; i < n; i++)
		tmp[i] = x[i];
	delete[] x;
	x = tmp;
	n *= 2;
}

int main()
{
	char command[MAX_COMMAND_LENGTH];
	char commands[][MAX_COMMAND_LENGTH] = {
		"PUSH",
		"POP",
		"PRINT_FORWARD",
		"PRINT_BACKWARD",
		"PRINT_QUEUE",
		"ADD_BEG",
		"ADD_END",

		"DEL_BEG",
		"DEL_END",
		"GARBAGE_SOFT",
		"GARBAGE_HARD",
	};
	srand(time(NULL));
	int i = 0;
	int length = sizeof(commands) / sizeof(commands[0]);
	int random = 0;
	int number = 0;
	Lista* k = new Lista;
	int xxx = 1;

	if (xxx==0)
	{
		while (i < SIZE)
		{
			number = rand() % 100;
			random = rand() % length;
			strcpy(command, commands[random]);
			printf("%s %d\n", command, number);
			//cout << "-----> " << i << "<-----" << endl;
			if (!strcmp("PUSH", command))
				k->Push2(number);
			else if (!strcmp("POP", command))
				k->Pop2();
			else if (!strcmp("PRINT_QUEUE", command))
				k->PrintQueue2();
			else if (!strcmp("PRINT_FORWARD", command))
				k->PrintForward();
			else if (!strcmp("PRINT_BACKWARD", command))
				k->PrintBackward();
			else if (!strcmp("ADD_BEG", command))
				k->AddBeg(number);
			else if (!strcmp("ADD_END", command))
				k->AddEnd(number);
			else if (!strcmp("DEL_BEG", command))
				k->DelBeg();
			else if (!strcmp("DEL_END", command))
				k->DelEnd();
			else if (!strcmp("GARBAGE_SOFT", command))
				k->GarbageSoft();
			else if (!strcmp("GARBAGE_HARD", command))
				k->GarbageHard();
			i++;
		}
	}

	if (xxx == 1)
	{
		int n = 10;
		Func* func = new Func[n];
		int rep = 0;
		while (scanf_s("%s", command, (unsigned)_countof(command)) == 1)
		{
			if (rep >= n)
				resize(func, n);
			if (!strcmp("PUSH", command))
			{
				func[rep].choice = 0;
				scanf("%d", &number);
				func[rep].val = number;
			}
			else if (!strcmp("POP", command))
				func[rep].choice = 1;

			else if (!strcmp("PRINT_QUEUE", command))
				func[rep].choice = 2;
			else if (!strcmp("PRINT_FORWARD", command))
				func[rep].choice = 3;
			else if (!strcmp("PRINT_BACKWARD", command))
				func[rep].choice = 4;
			else if (!strcmp("COUNT", command))
				func[rep].choice = 5;
			else if (!strcmp("SIZE", command))
				func[rep].choice = 6;
			else if (!strcmp("ADD_BEG", command))
			{
				func[rep].choice = 7;
				scanf("%d", &number);
				func[rep].val = number;
			}
			else if (!strcmp("ADD_END", command))
			{
				func[rep].choice = 8;
				scanf("%d", &number);
				func[rep].val = number;
			}
			else if (!strcmp("DEL_BEG", command))
				func[rep].choice = 9;
			else if (!strcmp("DEL_END", command))
				func[rep].choice = 10;
			else if (!strcmp("GARBAGE_SOFT", command))
				func[rep].choice = 11;
			else if (!strcmp("GARBAGE_HARD", command))
				func[rep].choice = 12;
			rep++;
		}
		Functions(k, func, n);
		delete func;
	}

	/*k->Push2(38);
	k->AddEnd(29);
	k->AddEnd(82);
	k->Push2(18);
	k->Push2(57);
	k->AddEnd(94);
	k->Pop2();
	k->AddEnd(56);
	k->Pop2();
	k->PrintForward();
	k->PrintQueue2();*/

	delete k;
}
