#include <iostream>
#include <string>

using namespace std;

class Node
{
public:
    int* keys;                    // tablica na klucze (wartosci)
    int t;                          // t - order   - steruje zakresami
    Node** childs = nullptr;        // dzieci
    int count_keys = 0;             // ile kluczy
    bool isLeaf = true;             // czy lisc
    Node(int t, bool isLeaf)
        :t(t), isLeaf(isLeaf)
    {
        keys = new int[2 * t - 1];
    }
    //Nie moze byc zapelniony
    void AddKeyToNode(int key)
    {
        keys[count_keys] = key;
        count_keys++;
    }
    //Nie moze byc zapelniony
    /*void AddChildToNode(Node* node)
    {
        if (childs == nullptr)
            childs = new Node * [2 * t];
        childs[count_childs] = node;
    }*/
    void Destructor(Node* node)
    {
        if (node)
        {
            if (node->keys)
                delete node->keys;
            if (node->isLeaf)
                return;
            else
            {
                for (int i = 0; i <= node->count_keys; i++)
                {
                    Destructor(node->childs[i]);
                    delete node->childs[i];
                }
                delete[] node->childs;
            }
        }
    }
};

class BTree
{
public:
    int sumaCache = 0;
    Node* root = nullptr;
    int t;
    BTree(int t)
        :t(t) {}
    BTree() {}
    ~BTree()
    {
        root->Destructor(root);
    }
    void Print(Node* node)
    {
        if (node->isLeaf)
        {
            for (int i = 0; i < node->count_keys; i++)
                cout << node->keys[i] << " ";
            return;
        }
        else
        {
            for (int i = 0; i <= node->count_keys; i++)
            {
                Print(node->childs[i]);
                if (i < node->count_keys)
                    cout << node->keys[i] << " ";
            }
        }
    }
    void Search(Node* node, int key)
    {
        int i = 0;
        while (i < node->count_keys && key > node->keys[i])
            i++;
        if (i < node->count_keys && key == node->keys[i])
            cout << key << " +" << endl;
        else if (node->isLeaf)
            cout << key << " -" << endl;
        else
            Search(node->childs[i], key);
    }
    Node* LoadTree(int t, string s)//, int& i, int& t
    {
        int childs = 0;
        int counter = 0;
        string digit = "";
        string s2 = "";     //"dzieli" stringa na mniejsze nawiasy,
                            // zamiast dzielenia dodaje po prostu do drugiego stringa
                            // jak counter nie jest 0 (bo wtedy tak wychodzi dobrze)
        Node* newNode = new Node(t, true);
        s = s.substr(1, s.size() - 2);
        for (int i = 0; i < s.length(); i++)
        {
            if (s[i] == '(')
                counter++;
            else if (s[i] == ')')
            {
                counter--;
                if (counter == 0)
                {
                    newNode->isLeaf = false;
                    if (newNode->childs == nullptr)
                        newNode->childs = new Node * [2 * t];
                    newNode->childs[childs] = LoadTree(t, s2 + s[i]);
                    childs++;
                    s2 = "";
                }
            }
            else if ('0' <= s[i] && s[i] <= '9' && counter == 0)
                digit += s[i];
            else if (digit != "" && s[i] == ' ' && counter == 0)
            {
                newNode->AddKeyToNode(stoi(digit));
                digit = "";
            }
            if (counter > 0)
                s2 += s[i]; //zamiast podzialu stringa (chyba inaczej sie w sumie nie da)
        }
        return newNode;
    }
    void SaveTree(Node* node)
    {
        cout << "( ";
        for (int i = 0; i <= node->count_keys; i++)
        {
            if (node->childs)
                SaveTree(node->childs[i]);
            if (i < node->count_keys)
                cout << node->keys[i] << " ";
        }
        cout << ") ";
    }
    void Insert(int newKey)
    {
        if (root == nullptr)
        {
            Node* newEl = new Node(t, true);
            root = newEl;
            newEl->count_keys++;
            newEl->keys = new int[2 * t - 1];
            newEl->keys[0] = newKey;
        }
        else
        {
            //Node* tmp = root;
            /*if (root->childs == nullptr)
            {*/
            if (root->count_keys == (2 * t - 1))
            {
                Node* newEl = new Node(t, false);
                //root = newEl;
                newEl->childs = new Node * [2 * t];
                newEl->childs[0] = root;
                //root->isLeaf = true; //??????
                SplitChildNode(0, newEl, root);
                int i = 0;
                if (newEl->keys[0] < newKey)
                    i = 1;
                InsertNonFull(newEl->childs[i], newKey);
                root = newEl;
            }
            else
                InsertNonFull(root, newKey);
            //}
        }
    }
    //Node musi byc pelny zeby funkcja dzialala
    void SplitChildNode(int index, Node* parent, Node* nodeToSplit) //index w tablicy dzieci rodzica
    {
        Node* newNode = new Node(nodeToSplit->t, nodeToSplit->isLeaf);
        newNode->count_keys = t - 1;

        //Skopiowanie ostatnich kluczy (t-1) z nodeToSplit do newNode
        for (int i = 0; i < t - 1; i++)
            newNode->keys[i] = nodeToSplit->keys[i + t]; // prawa strona node'a , t-1 bo to srodek

        //Skopiowanie ostatnich dzieci (t) z nodeToSplit do newNode
        if (newNode->isLeaf == false)                        //mozna to wrzucic do petli wyzej
        {
            newNode->childs = new Node * [2 * t];
            for (int i = 0; i < t; i++)
                newNode->childs[i] = nodeToSplit->childs[i + t];
        }

        //Zmniejsz wskaznik liczby kluiczy w nodeToSplit
        nodeToSplit->count_keys = t - 1;

        //Zrob miejsce na nowe dziecko (node) u rodzica (parent)
        // *** childs o indeksie "index" zostaje w miejscu!!! ***
        for (int i = parent->count_keys; i >= index + 1; i--)
            parent->childs[i + 1] = parent->childs[i];

        //Dolacz nowe dziecko do parenta (.)A.B.C. -> A(.)B.C...
        parent->childs[index + 1] = newNode;

        //PRzesun klucze parenta o 1 w prawo
        for (int i = parent->count_keys - 1; i >= index; i--)
            parent->keys[i + 1] = parent->keys[i];

        //Skopiuj klucz z nodeToSplit (najbardziej na prawo) do ojca
        // w wolne pole (po przesunieciu jest wolne)
        parent->keys[index] = nodeToSplit->keys[t - 1];

        // Zwieksz liczbe kluczy w ojcu
        parent->count_keys++;
    }
    void InsertNonFull(Node* node, int newKey)
    {
        int i = node->count_keys;
        if (node->isLeaf)  //Jesli jest lisciem
        {
            //Znajdz miejsce na nowy klucz i
            //przesun wszystkie wieksze klucze w prawo
            while (i > 0 && newKey < node->keys[i - 1])
            {
                node->keys[i] = node->keys[i - 1];
                i--;
            }
            node->keys[i] = newKey;
            node->count_keys++;
        }
        else                //jesli nie jest lisciem
        {
            // znajdz dziecko ktore ma dostac
            while (i > 0 && newKey < node->keys[i - 1])
                i--;
            //i++;
            if (node->childs[i]->count_keys == (2 * t - 1))
            {
                SplitChildNode(i, node, node->childs[i]);
                if (newKey > node->keys[i])
                    i++;
            }
            InsertNonFull(node->childs[i], newKey);
        }
    }
    //Zwraca index klucza lub index wiekszego niz klucz o 1
    int FindKey(Node* root, int k)
    {
        int index = 0;
        while (index < root->count_keys && root->keys[index] < k)
            index++;
        return index;
    }
    //void RemoveFromLeaf(int removeKey)
    void Remove(int removeKey)
    {
        RemoveKey(root, removeKey);
        if (root->count_keys == 0)
        {
            Node* tmp = root;
            if (root->isLeaf)
                root = nullptr;
            else
                root = root->childs[0];
            delete tmp;
        }
    }
    void RemoveKey(Node* node, int key)
    {
        int index = FindKey(node, key);

        //jesli klucz jest w tym node
        if (index < node->count_keys && node->keys[index] == key)
        {
            if (node->isLeaf)
                RemoveFromLeaf(node, index);
            else
                RemoveFromNonLeaf(node, index);
        }
        //jesli klucza  w tym node nie ma
        else
        {
            //jesli nie jest lisciem (jak jest to nie znaleziono klcza)
            if (!node->isLeaf)
            {
                //flaga - jesli index wskazuje na ostatnie dziecko node'a to jest flaga
                // uzywana w tym jak uzupelniamy dziecko (fill) i przesuna sie nam dzieci w 
                // node to dzieki fladze wiemy ze chodzi o ostatnie dziecko
                bool flag = ((index == node->count_keys) ? true : false);

                //jesli dziecko w ktorym mialby byc klucz ktory chcemy usunac ma 
                //minimalna liczbe dzieci to uzupelnij te dziecko
                if (node->childs[index]->count_keys < t)
                    Fill(node, index);

                if (flag && index > node->count_keys)
                    RemoveKey(node->childs[index - 1], key);
                else
                    RemoveKey(node->childs[index], key);
            }
        }
    }
    void RemoveFromLeaf(Node* node, int index)
    {
        for (int i = index; i < node->count_keys-1; i++)
            node->keys[i] = node->keys[i + 1];
        node->count_keys--;
    }
    void RemoveFromNonLeaf(Node* node, int index)
    {
        int k = node->keys[index];
        
        //jesli dziecko przed kluczem ktory usuwamy ma wiecej niz minimalna
        //liczbe kluczy to jest git, bo normalnie mozemy przesunac ostatni
        //klcuz dziecka w miejsce usuwanego klucza
        if (node->childs[index]->count_keys >= t)
        {
            node->keys[index] = GetPrev(node, index);
            RemoveKey(node->childs[index], node->keys[index]);
        }
        //analogicznie do if'a wyzej, ale odwrotnie
        //jesli w dziecku po prawej jest przynajmniej t kluczy
        else if (node->childs[index + 1]->count_keys >= t)
        {
            node->keys[index] = GetNext(node, index);
            RemoveKey(node->childs[index + 1], node->keys[index]);
        }
        //jesli dzieci obok node'a nie maja minimum 't' dzieci
        else
        {
            Merge(node, index);
            RemoveKey(node->childs[index], k);
        }
    }
    //Uzupelnij dziecko node'a o indeksie 'index' ktory ma t-1 kluczy 
    void Fill(Node* node, int index)
    {
        if (index != 0 && node->childs[index - 1]->count_keys > (t - 1))
            BorrowLeft(node, index);
        else if (index != node->count_keys && node->childs[index + 1]->count_keys > (t - 1))
            BorrowRight(node, index);
        else
        {
            if (index == node->count_keys) //ostatnie dziecko node'a
                Merge(node, index - 1);    //Merge bierze dziecko, i jego PRAWEGO brata
            else
                Merge(node, index);
        }
    }
    //oba dzieci maja minimum kluczy
    void Merge(Node* parent, int index)
    {
        Node* child = parent->childs[index];        
        Node* brother = parent->childs[index+1];  // prawy brat
        
        //zabranie wartosci z gory i wklejenie w srodek pomiedzy node'y
        child->keys[t - 1] = parent->keys[index];

        //przenies kluczy z brata do child
        for (int i = 0; i < t-1; i++) // ++i ###########
            child->keys[t + i] = brother->keys[i]; // t+i a nie t-1+i bo doklejony srodek
        
        //przeniesienie dzieci brata do child
        if (!child->isLeaf)
            for (int i = 0; i < t; i++) //++i ###########
                child->childs[t + i] = brother->childs[i];

        //uzupelneinie luki ktora powstala po zabraniu elementu z parent
        //kluczami
        for (int i = index; i < parent->count_keys-1; i++)
            parent->keys[i] = parent->keys[i + 1];

        //przesuniecie dzieci w parent
        for (int i = index+1; i < parent->count_keys; i++)
            parent->childs[i] = parent->childs[i + 1]; //#######

        //zaktualizuj wskaznik
        child->count_keys = 2 * t - 1;
        parent->count_keys--;        
        delete brother;
    }
    void BorrowLeft(Node* parent, int index)  //index prawego dziecka
    {
        Node* leftChild = parent->childs[index - 1];
        Node* rightChild = parent->childs[index];

        //zakladam ze left child nie ma minimalnej liczby kluczy
        //przesuwam klucze o 1 w prawo (miejsce dla klucza robie)
        for (int i = t - 1; i > 0; i--)
            rightChild->keys[i] = rightChild->keys[i - 1];
        rightChild->keys[0] = parent->keys[index-1];

        //przesuwam dzieci w prawym dziecku o 1 w prawo
        //i przypisuje ostatnie dziecko LEWEGO dziecka do pierwszego PRAWEGO dziecka
        if (!rightChild->isLeaf)
        {
            for (int i = t; i > 0; i--)
                rightChild->childs[i] = rightChild->childs[i - 1];
            rightChild->childs[0] = leftChild->childs[leftChild->count_keys];
        }

        //wstawiam klucz z lewego dziecka w miejsce starego klucza ojca
        parent->keys[index-1] = leftChild->keys[leftChild->count_keys - 1];
        
        leftChild->count_keys--;
        rightChild->count_keys++;
    }
    void BorrowRight(Node* parent, int index)  //index lewego dziecka
    {
        Node* leftChild = parent->childs[index];
        Node* rightChild = parent->childs[index+1];

        leftChild->keys[t-1] = parent->keys[index];

        //przypisuje pierwsze dziecko PRAWEGO dziecka do ostatniego LEWEGO dziecka
        //przesuwam dzieci w prawym dziecku o 1 w lewo
        if (!leftChild->isLeaf)
        {
            leftChild->childs[t] = rightChild->childs[0];
            for (int i = 0; i < rightChild->count_keys; i++)
                rightChild->childs[i] = rightChild->childs[i+1];
        }

        //wstawiam klucz z lewego dziecka w miejsce starego klucza ojca
        parent->keys[index] = rightChild->keys[0];
        //zakladam ze left child nie ma minimalnej liczby kluczy
        //przesuwam klucze o 1 w prawo (miejsce dla klucza robie)
        for (int i = 0; i < rightChild->count_keys-1; i++)
            rightChild->keys[i] = rightChild->keys[i + 1];

        leftChild->count_keys++;
        rightChild->count_keys--;
    }
    //zwraca wartosc poprzednika/nastepnika klucza o indeksie 'index'
    int GetPrev(Node* node, int index)
    {
        Node* curr = node->childs[index];
        while (!curr->isLeaf)
            curr = curr->childs[curr->count_keys];
        return curr->keys[curr->count_keys - 1];
    }
    int GetNext(Node* node, int index)
    {
        Node* curr = node->childs[index+1];
        while (!curr->isLeaf)
            curr = curr->childs[0];
        return curr->keys[0];
    }
};

class Fifo {
public:
    struct FifoEl {
        int* value = nullptr;
        FifoEl* next = nullptr;
    };
    FifoEl* first = nullptr;
    int CacheCount = 0;
    int size = 0;
    int counter = 0;

    Fifo(int size)
        :size(size){}
    ~Fifo()
    {
        if (first)
        {
            FifoEl* curr = first;
            FifoEl* tmp;
            do {
                tmp = curr->next;
                delete curr->value;
                delete curr;
                curr = tmp;
            } while (tmp);
        }
    }

    void AddValue(int value)
    {
        FifoEl* newEl = new FifoEl();
        if (first)
        {
            if (CheckValue(value))
                return;
            else
            {
                if (size == counter)
                    DelLastValue();
                FifoEl* curr = first;
                FifoEl* next = first->next;
                while (next)
                {
                    curr = next;
                    next = next->next;
                }
                curr->next = newEl;
            }
        }
        else
            first = newEl;
        newEl->value = new int(value);
        counter += 1;
    }
    void DelLastValue()
    {
        if (first)
        {
            FifoEl* next = first->next;
            FifoEl* curr = first;
            FifoEl* prev = first;
            while (next)
            {
                curr = next;
                if (curr->next == nullptr)
                    break;
                next = next->next;
                prev = curr;
            }
            prev->next = nullptr;
            delete curr->value;
            delete curr;   
            counter -= 1;
            if (counter == 0)
                first = nullptr;
        }
    }
    void ValuesFromString(string s, int& cacheBtree)
    {
        string digit = "";
        for (int i = 0; i <= s.length(); i++)
        {
            if ('0' <= s[i] && s[i] <= '9')
                digit += s[i];
            else if (digit != "" && (s[i] == ' ' || i == s.length()))
            {
                cacheBtree++;
                AddValue(stoi(digit));
                digit = "";
            }
        }
    }
    bool CheckValue(int value)
    {
         FifoEl* tmp = first;
         while (tmp)
         {
             if (*tmp->value == value)
                 return true;
             tmp = tmp->next;
         }
         CacheCount++;
         return false;
    }
    void ListValues()
    {
        FifoEl* curr = first;
        while (curr)
        {
            cout << *curr->value << " ";
            curr = curr->next;
        }
        cout << endl;
    }
};

int main()
{
    BTree* t = new BTree();
    Fifo* f = nullptr;
    char c[5000];
    int x[5000];
    char l[5000];
    string s;
    int i = 0;
    int k = 0;
    int value;
    while (true)
    {
        cin >> c[i];
        if (c[i] == 'X')
            break;
        else if (c[i] == 'I' || c[i] == 'A' || c[i] == '?' || c[i] == 'R')
        {
            cin >> x[i];
            i++;
        }
        else if (c[i] == 'P')
            i++;
        else if (c[i] == 'S')
            i++;
        else if (c[i] == 'L')
        {
            cin >> x[i];
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, s);
            t->t = x[i];
            t->root = t->LoadTree(t->t, s);
            i++;
        }
        else if (c[i] == 'C')
        {
            int btreecache = 0;
            cin >> x[i];
            f = new Fifo(x[i]);
            //cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, s);
            f->ValuesFromString(s, btreecache);
            f->ListValues();
            t->sumaCache += btreecache;
            cout << "NO CACHE: " << btreecache << " CACHE: " << f->CacheCount << endl;
        }
    }
    for (int j = 0; j < i; j++)
    {
        if (c[j] == 'I')
            t->t = x[j];
        else if (c[j] == 'A')
            t->Insert(x[j]);
        else if (c[j] == '?')
            t->Search(t->root, x[j]);
        else if (c[j] == 'P')
        {
            t->Print(t->root);
            cout << endl;
        }
        else if (c[j] == 'S')
        {
            cout << t->t << endl;
            t->SaveTree(t->root);
        }
        else if (c[j] == 'R')
            t->Remove(x[j]);
        else
            continue;
    }
    f->ListValues();
    if (f)
        delete f;

    /*BTree* t = new BTree();
    t->root = t->LoadTree(3, "( ( ( 1 2 ) 3 ( 4 5 6 ) 7 ( 8 9 10 ) 11 ( 12 13 ) ) 14 ( ( 15 16 17 ) 18 ( 19 20 ) 21 ( 22 23 ) ) )");
    t->t = 3;
    t->Remove(6);
    t->Remove(11);
    t->SaveTree(t->root); cout << endl;
    t->Remove(7);
    t->SaveTree(t->root); cout << endl;
    t->Remove(4);
    t->Print(t->root); cout << endl;
    t->Remove(2);
    t->Print(t->root); cout << endl;

    delete t;*/

}
