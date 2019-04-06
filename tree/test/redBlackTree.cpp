#include <iostream>
#include <cstdlib>

#include "redBlackTree.h"

using namespace std;

typedef int Key;
typedef int Value;
typedef RedBlackTree<Key, Value>::node_type NodeType;
typedef RedBlackTree<Key, Value>::node_ptr NodePtr;
typedef RedBlackTree<Key, Value>::elem_type ElemType;
typedef RedBlackTree<Key, Value>::elem_ptr ElemPtr;
typedef RedBlackTree<Key, Value>::color_type ColorType;

void output(NodeType * node)
{
    cout << " (" << node->element.first << ", " 
        << node->element.second << ")";

    if (node->color == ColorType::RED)
        cout << "-";
    else
        cout << "+";

    cout << node->blackCount;
}

void printTree(RedBlackTree<Key, Value> & t)
{
    cout << t.height() << " pre:  ";
    t.preOrder(output);
    cout << endl << t.height() << " in:   ";
    t.inOrder(output);
    //cout << endl << t.height() << " post: ";
    //t.postOrder(output);
    cout << endl;
}

int main()
{
    static int array[] = {3, 1, 8, 9, 7, 4, 6, 5};
    static int size = sizeof(array) / sizeof (int);

    RedBlackTree<Key, Value> t;
    for (int i = 0; i < size; i++)
    {
        t.insert(array[i], array[i]);
        printTree(t);
    }

    cout << endl;

    for (int i = 0; i < size; i++)
    {
        RedBlackTree<Key, Value>::elem_ptr e = t.find(array[i]);
        if (e != NULL)
            cout << "(" << e->first << ", " << e->second << ")  ";
        else
            cout << array[i] << " not found  ";
    }

    cout << endl << endl;

    for (int i = 0; i < size; i++)
    {
        t.erase(array[i]);
        printTree(t);
    }

    return 0;
}
