#include <iostream>
#include <cstdlib>

#include "binarySearchTree.h"

using namespace std;

typedef int Key;
typedef int Value;
typedef BinaryTreeNode<pair<const Key, Value> > NodeType;

void output(NodeType * node)
{
    cout << "(" << node->element.first << ", " 
        << node->element.second << ")";
}

int main()
{
    static int array[] = {0, 1, 5, 6, 8, 2, 4};
    static int size = sizeof(array) / sizeof (int);

    BinarySearchTree<Key, Value> t;
    for (int i = 0; i < size; i++)
    {
        t.insert(array[i], array[i]);
        
        t.inOrder(output);
        cout << endl;
    }

    cout << endl;

    for (int i = 0; i < size; i++)
    {
        t.erase(array[i]);

        t.inOrder(output);
        cout << endl;
    }

    return 0;
}
