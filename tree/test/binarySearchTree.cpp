#include <iostream>
#include <cstdlib>

#include "binarySearchTree.h"

using namespace std;

typedef int Key;
typedef int Value;
typedef BinarySearchTree<Key, Value>::node_type NodeType;
typedef BinarySearchTree<Key, Value>::node_ptr NodePtr;

void output(NodePtr node)
{
    cout << " (" << node->element.first << ", " 
        << node->element.second << ")";
}

void printTree(BinarySearchTree<Key, Value> & t)
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
    static int array[] = {0, 1, 5, 6, 8, 2, 4};
    static int size = sizeof(array) / sizeof (int);

    BinarySearchTree<Key, Value> t;
    for (int i = 0; i < size; i++)
    {
        t.insert(array[i], array[i]);
        printTree(t);
    }

    cout << endl;

    for (int i = 0; i < size; i++)
    {
        BinarySearchTree<Key, Value>::elem_ptr e = t.find(array[i]);
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
