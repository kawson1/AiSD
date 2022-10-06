#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define MAX_COMMAND_LENGTH 15
#define SIZE 100000



using namespace std;

struct Data {
    int liczba;
    Data* npx;
};

struct Node {
    Data* first = NULL; //PREV w przypadku act
    Data* last = NULL; //NEXT w przypadku act
    Data* act;
};

struct Func {
    int n;
    int choice;
};


struct Data* XOR(struct Data* prev, struct Data* next)
{
    return (Data*)((uintptr_t)(prev) ^ (uintptr_t)(next));
}

void Actual(Node* actual)
{
    if (actual == NULL)
        cout << "NULL" << endl;
    else
        cout << actual->act->liczba << endl;
}

void Next(Node*& actual, Node* fl, bool print)
{
    if (actual == NULL)
        cout << "NULL" << endl;
    else
    {
        Data* tmp = actual->act;
        actual->act = actual->last;
        if (actual->act == NULL)
        {
            actual->act = fl->first;
            actual->first = NULL;
            actual->last = XOR(NULL, actual->act->npx);
        }
        else
        {
            actual->first = tmp;
            actual->last = XOR(tmp, actual->act->npx);
        }
        if (print)
            Actual(actual);
    }
}

void Prev(Node*& actual, Node*& fl, bool print)
{
    if (actual == NULL)
        cout << "NULL" << endl;
    else
    {
        Data* tmp = actual->act;
        actual->act = actual->first;
        if (actual->act == NULL)
        {
            actual->act = fl->last;
            actual->last = NULL;
            actual->first = XOR(actual->act->npx, NULL);
        }
        else
        {
            actual->first = XOR(actual->act->npx, tmp);
            actual->last = tmp;
        }
        if (print)
            Actual(actual);
    }
}

void PushEnd(Node*& fl, Node*& actual, int num)
{
    Data* tmp = new Data;
    tmp->liczba = num;
    tmp->npx = XOR(fl->last, NULL);
    if (fl->last != NULL)
    {
        if (XOR(fl->last, NULL) == actual->act)
            actual->last = tmp;
        fl->last->npx = XOR(XOR(fl->last->npx, NULL), tmp);
    }
    else
    {
        actual = new Node;
        fl->first = tmp;
        actual->first = NULL;
        actual->last = NULL;
        actual->act = tmp;
    }
    fl->last = tmp;
}

void PushBeg(Node*& fl, Node*& actual, int num)
{
    Data* tmp = new Data;
    tmp->liczba = num;
    tmp->npx = XOR(NULL, fl->first);
    if (fl->first != NULL)
    {
        if (XOR(NULL, fl->first) == actual->act)
            actual->first = tmp;
        fl->first->npx = XOR(tmp, XOR(NULL, fl->first->npx));
    }
    else
    {
        actual = new Node;
        fl->last = tmp;
        actual->first = NULL;
        actual->last = NULL;
        actual->act = tmp;
    }
    fl->first = tmp;
}

void DelBeg(Node*& fl, Node*& actual)
{
    if (fl->first != NULL)
    {
        if (fl->last == fl->first)
        {
            delete fl->last;
            fl->first = NULL;
            fl->last = NULL;
            delete actual;
            actual = nullptr;
        }
        else
        {
            if (actual->act == fl->first)
                Next(actual, fl, 0);
            if (actual->first == fl->first)
                actual->first = NULL;
            Data* tmp = fl->first;
            fl->first = XOR(NULL, fl->first->npx);
            fl->first->npx = XOR(tmp, fl->first->npx);
            delete tmp;
        }
    }
}

void DelEnd(Node*& fl, Node*& actual)
{
    if (fl->first != NULL)
    {
        if (fl->last == fl->first)
        {
            delete fl->last;
            fl->first = NULL;
            fl->last = NULL;
            delete actual;
            actual = nullptr;
        }
        else
        {
            if (actual->act == fl->last)
                Prev(actual, fl, 0);
            if (actual->last == fl->last)
                actual->last = NULL;
            Data* tmp = fl->last;
            fl->last = XOR(fl->last->npx, NULL);
            fl->last->npx = XOR(fl->last->npx, tmp);
            delete tmp;
        }
    }
}

void DelAct(Node*& fl, Node*& actual)
{
    if (actual != NULL)
    {
        if (actual->act == fl->first || actual->act == fl->last)
        {
            if (actual->act == fl->first)
            {
                DelBeg(fl, actual);
                if (actual != NULL)
                    Prev(actual, fl, 0);
            }
            else
                DelEnd(fl, actual);
        }
        else
        {
            Data* tmp = actual->act;
            Data* next;
            Data* pre;
            Prev(actual, fl, 0);
            pre = XOR(actual->act->npx, tmp); //PRZED ACTUALEM
            next = XOR(actual->act, tmp->npx); //PO TEMPIE
            actual->act->npx = XOR(pre, next);
            actual->last = next;
            next->npx = XOR(actual->act, XOR(tmp, next->npx));
            delete tmp;
        }
    }
}

void DelVal(Node*& fl, Node*& actual, int n)
{
    if (fl->first != NULL)
    {
        Node* tmp = new Node;
        tmp->act = fl->first;
        tmp->first = NULL;
        tmp->last = XOR(NULL, tmp->act->npx);
        do
        {
            if (tmp->act->liczba == n)
            {
                if (tmp->act == fl->last)
                    DelEnd(fl, actual);
                else
                {
                    if (actual->last = tmp->act)
                        actual->last = tmp->last;
                    if (actual->first = tmp->act)
                        actual->first = tmp->first;
                    if (actual->act = tmp->act)
                    {
                        Prev(actual, fl, 0);
                        actual->last = tmp->last;
                    }
                    DelAct(fl, tmp);
                    Next(tmp, fl, 0);
                }
            }
            if (fl->first != NULL)
                Next(tmp, fl, 0);
        } while (tmp->act != fl->first && fl->first != NULL);
    }
}

void AddAct(Node*& fl, Node*& actual, int n)
{
    if (actual == NULL)
        PushBeg(fl, actual, n);
    else
    {
        if (actual->act == fl->first)
            PushBeg(fl, actual, n);
        else
        {
            Data* tmp = new Data; //DODAWANY ELEMENT PRZED ACT
            Data* prev = actual->first; //PRZED TMP    <prev> - <TMP> - <act>
            tmp->liczba = n;
            tmp->npx = XOR(prev, actual->act);
            prev->npx = XOR(XOR(prev->npx, actual->act), tmp);
            actual->act->npx = XOR(tmp, actual->last);
            actual->first = tmp;
        }
    }
}

void PrintBackward(Node* fl)
{
    if (fl->first == NULL)
        cout << "NULL" << endl;
    else
    {
        Data* next;
        Data* curr = fl->last;
        Data* prev = NULL;
        while (curr)
        {
            cout << curr->liczba << " ";
            next = XOR(curr->npx, prev);
            prev = curr;
            curr = next;
        }
        cout << endl;
    }
}

void PrintForward(Node* fl)
{
    if (fl->first == NULL)
        cout << "NULL" << endl;
    else
    {
        Data* next;
        Data* curr = fl->first;
        Data* prev = NULL;
        while (curr)
        {
            cout << curr->liczba << " ";
            next = XOR(prev, curr->npx);
            prev = curr;
            curr = next;
        }
        cout << endl;
    }
}

void DeleteAll(Node* fl, Node* actual)
{
    if (fl->first == NULL)
    {
        delete fl;
    }
    else
    {
        Data* curr = fl->last;
        Data* tmp = NULL;
        Data* next;
        while (curr)
        {
            next = XOR(curr->npx, tmp);
            tmp = curr;
            delete curr;
            curr = next;
        }
        delete fl;
        delete actual;
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

void Functions(Func* x, int n, Node*& fl, Node*& actual)
{
    for (int j = 0; j < n; j++)
    {
        if (x[j].choice == 1)
            Actual(actual); if (x[j].choice == 2)
            Next(actual, fl, 1); if (x[j].choice == 3)
            Prev(actual, fl, 1); if (x[j].choice == 4)
            PushBeg(fl, actual, x[j].n); if (x[j].choice == 5)
            PushEnd(fl, actual, x[j].n); if (x[j].choice == 6)
            AddAct(fl, actual, x[j].n); if (x[j].choice == 7)
            DelBeg(fl, actual); if (x[j].choice == 8)
            DelEnd(fl, actual); if (x[j].choice == 9)
            DelVal(fl, actual, x[j].n); if (x[j].choice == 10)
            DelAct(fl, actual); if (x[j].choice == 11)
            PrintForward(fl); if (x[j].choice == 12)
            PrintBackward(fl);
    }
}

int main()
{
    //Node* fl = new Node;
    //Node* actual = NULL;
    //int size = 2;
    //Func* x = new Func[size];
    //int i = 0;
    //int n = 0;
    ////char com[15];


    //char com[MAX_COMMAND_LENGTH];
    //int number;
    ////LinkedList list;
    //char commands[][MAX_COMMAND_LENGTH] = {
    //    "ADD_BEG",
    //    "ADD_END",
    //    "ACTUAL",
    //    "NEXT",
    //    "PREV",
    //    "DEL_BEG",
    //    "DEL_END",
    //    "DEL_ACT",
    //    "PRINT_FORWARD",
    //    "PRINT_BACKWARD",
    //    "ADD_ACT",
    //    "DEL_VAL"
    //};

    //int xxx = 0;
    //int length = sizeof(commands) / sizeof(commands[0]);
    //int random;
    //srand(time(NULL));

    //while (xxx < SIZE) {
    //    number = rand() % SIZE;
    //    random = rand() % length;
    //    strcpy(com, commands[random]);
    //    printf("%s\n", commands[random]);

    //    //while (scanf_s("%s", com, (unsigned)_countof(com)) == 1)
    //    {
    //        if (i >= size)
    //            resize(x, size);
    //        if (strstr(com, ("ACTUAL")))
    //        {
    //            x[i].choice = 1;
    //        }if (strstr(com, ("NEXT")))
    //        {
    //            x[i].choice = 2;
    //        }if (strstr(com, ("PREV")))
    //        {
    //            x[i].choice = 3;
    //        }if (strstr(com, ("ADD_BEG")))
    //        {
    //            x[i].choice = 4;
    //            //cin >> n;
    //            x[i].n = number;
    //        }if (strstr(com, ("ADD_END")))
    //        {
    //            x[i].choice = 5;
    //            //cin >> n;
    //            x[i].n = number;
    //        }if (strstr(com, ("ADD_ACT")))
    //        {
    //            x[i].choice = 6;
    //            //cin >> n;
    //            x[i].n = number;
    //        }if (strstr(com, ("DEL_BEG")))
    //        {
    //            x[i].choice = 7;
    //        }if (strstr(com, ("DEL_END")))
    //        {
    //            x[i].choice = 8;
    //        }if (strstr(com, ("DEL_VAL")))
    //        {
    //            x[i].choice = 9;
    //            //cin >> n;
    //            x[i].n = number;
    //        }if (strstr(com, ("DEL_ACT")))
    //        {
    //            x[i].choice = 10;
    //        }if (strstr(com, ("PRINT_FORWARD")))
    //        {
    //            x[i].choice = 11;
    //        }if (strstr(com, ("PRINT_BACKWARD")))
    //        {
    //            x[i].choice = 12;
    //        }
    //        i++;
    //        xxx++;
    //    }
    //}
    //Functions(x, i, fl, actual);

    //DeleteAll(fl, actual);
    ////delete[] x;
    int* x = new int[2];
    int* y = new int[2];
    x[0] = 1;
    x[1] = 2;
    y[0] = x[0];
    y[1] = x[1];
    y[0]++;
    cout << x[0] << " " << y[0];

    delete[] x;
    delete[] y;
}