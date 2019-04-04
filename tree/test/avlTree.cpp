#include <iostream>
#include <cstdlib>

#include "avlTree.h"

using namespace std;

typedef int Key;
typedef int Value;
typedef AVLTree<Key, Value>::node_type NodeType;
typedef AVLTree<Key, Value>::node_ptr NodePtr;
typedef AVLTree<Key, Value>::elem_type ElemType;
typedef AVLTree<Key, Value>::elem_ptr ElemPtr;

void output(NodeType * node)
{
    cout << " (" << node->element.first << ", " 
        << node->element.second << ")";
}

void printTree(AVLTree<Key, Value> & t)
{
    cout << t.height() << " pre:  ";
    t.preOrder(output);
    cout << endl << t.height() << " in:   ";
    t.inOrder(output);
    cout << endl << t.height() << " post: ";
    t.postOrder(output);
    cout << endl;
}

int main()
{
    static int array[] = {0, 1, 5, 6, 8, 2, 4};
    static int size = sizeof(array) / sizeof (int);

    AVLTree<Key, Value> t;
    for (int i = 0; i < size; i++)
    {
        printTree(t);
        t.insert(array[i], array[i]);
    }

    printTree(t);
    cout << endl;

    for (int i = 0; i < size; i++)
    {
        AVLTree<Key, Value>::elem_ptr e = t.find(array[i]);
        if (e != NULL)
            cout << "(" << e->first << ", " << e->second << ")  ";
        else
            cout << array[i] << " not found  ";
    }

    cout << endl << endl;

    for (int i = 0; i < size; i++)
    {
        printTree(t);
        t.erase(array[i]);
    }

    printTree(t);

    return 0;
}
