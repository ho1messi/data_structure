#include <iostream>
#include <cstdlib>

#include "bTree.h"

using namespace std;

const size_t N = 3;

typedef int Key;
typedef int Value;
typedef BTree<Key, Value, N>::node_type NodeType;
typedef BTree<Key, Value, N>::node_ptr NodePtr;
typedef BTree<Key, Value, N>::elem_type ElemType;
typedef BTree<Key, Value, N>::elem_ptr ElemPtr;

void output(NodeType * node)
{
    cout << " (" << node->elements[0]->first;

    size_t index = 1;
    while (node->elements[index] != NULL)
        cout << ", " << node->elements[index++]->first;

    cout << ")";
}

void printTree(BTree<Key, Value, N> & t)
{
    cout << t.height() << " pre:  ";
    t.preOrder(output);
    cout << endl << t.height() << " post: ";
    t.postOrder(output);
    cout << endl;
}

int main()
{
    static int insertList[] = {4, 3, 8, 9, 7, 5, 6};
    static int eraseList[] = {8, 6, 7, 5, 3, 4, 9};
    static int size = sizeof(insertList) / sizeof (int);

    BTree<Key, Value, N> t;
    for (int i = 0; i < size; i++)
    {
        t.insert(insertList[i], insertList[i]);
        printTree(t);
    }

    cout << endl;

    for (int i = 0; i < size; i++)
    {
        BTree<Key, Value, N>::elem_ptr e = t.find(insertList[i]);
        if (e != NULL)
            cout << "(" << e->first << ", " << e->second << ")  ";
        else
            cout << insertList[i] << " not found  ";
    }

    cout << endl << endl;

    for (int i = 0; i < size; i++)
    {
        t.erase(eraseList[i]);
        printTree(t);
    }

    return 0;
}
