#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define MAX_COMMAND_LENGTH 15
#define SIZE 100000

using namespace std;

int main() {

	char command[MAX_COMMAND_LENGTH];
	int number;
	//LinkedList list;
	char commands[][MAX_COMMAND_LENGTH] = {
		"ADD_BEG",
		"ADD_END",
		"ACTUAL",
		"NEXT",
		"PREV",
		"DEL_BEG",
		"DEL_END",
		"DEL_ACT",
		"PRINT_FORWARD",
		"PRINT_BACKWARD",
		"ADD_ACT",
		"DEL_VAL"
	};

	int i = 0;
	int length = sizeof(commands) / sizeof(commands[0]);
	int random;
	srand(time(NULL));

	while (i < SIZE) {
		number = rand() % SIZE;
		random = rand() % length;
		strcpy(command, commands[random]);
		printf("%s\n", commands[random]);
		if (!strcmp("ADD_BEG", command)) {
			x[i].choice = 1;
		}
		else if (!strcmp("ADD_END", command)) {
			list.addLast(number);
		}
		else if (!strcmp("ACTUAL", command)) {
			Node* actual = list.getActual();
			if (actual != nullptr)
				printf("%d\n", actual->getKey());
			else
				printf("NULL\n");
		}
		else if (!strcmp("NEXT", command)) {
			Node* next = list.getNext();
			if (next != nullptr)
				printf("%d\n", next->getKey());
			else
				printf("NULL\n");
		}
		else if (!strcmp("PREV", command)) {
			Node* previous = list.getPrevious();
			if (previous != nullptr)
				printf("%d\n", previous->getKey());
			else
				printf("NULL\n");
		}
		else if (!strcmp("DEL_BEG", command)) {
			list.deleteFirst();
		}
		else if (!strcmp("DEL_END", command)) {
			list.deleteLast();
		}
		else if (!strcmp("DEL_ACT", command)) {
			list.deleteActual();
		}
		else if (!strcmp("PRINT_FORWARD", command)) {
			list.printForward();
		}
		else if (!strcmp("PRINT_BACKWARD", command)) {
			list.printBackward();
		}
		else if (!strcmp("ADD_ACT", command)) {
			list.addActual(number);
		}
		else if (!strcmp("DEL_VAL", command)) {
			list.deleteValues(number);
		}
		i++;
	}
	return 0;
}