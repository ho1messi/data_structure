#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

#include <list>
#include <algorithm>
#include <cstddef>

template <class T>
struct BinaryTreeNode
{
    T element;
    BinaryTreeNode<T> *leftChild, *rightChild;

    BinaryTreeNode()
        : leftChild(NULL), rightChild(NULL) {}

    BinaryTreeNode(const T & element)
        : element(element), leftChild(NULL), rightChild(NULL) {}

    BinaryTreeNode(const T & element, BinaryTreeNode * leftChild, 
                   BinaryTreeNode * rightChild)
        : element(element), leftChild(leftChild), rightChild(rightChild) {}
};

template <class T>
class BinaryTree
{
public:
    typedef size_t size_type;

public:
    
    virtual ~BinaryTree() {}

    virtual bool empty() const = 0;

    virtual size_type size() const = 0;

    virtual void preOrder(void (*) (BinaryTreeNode<T> *)) = 0;

    virtual void inOrder(void (*) (BinaryTreeNode<T> *)) = 0;

    virtual void postOrder(void (*) (BinaryTreeNode<T> *)) = 0;

    virtual void levelOrder(void (*) (BinaryTreeNode<T> *)) = 0;
};

template <class T>
typename BinaryTree<T>::size_type
height(BinaryTreeNode<T> * t)
{
    if (t == NULL)
        return 0;

    typename BinaryTree<T>::size_type l = height(t->leftChild);
    typename BinaryTree<T>::size_type r = height(t->rightChild);

    return std::max(l, r) + 1;
}

template <class T>
void preOrder(BinaryTreeNode<T> * t, void (* visit) (BinaryTreeNode<T> *))
{
    if (t != NULL)
    {
        visit(t);
        preOrder(t->leftChild, visit);
        preOrder(t->rightChild, visit);
    }
}

template <class T>
void inOrder(BinaryTreeNode<T> * t, void (* visit) (BinaryTreeNode<T> *))
{
    if (t != NULL)
    {
        inOrder(t->leftChild, visit);
        visit(t);
        inOrder(t->rightChild, visit);
    }
}

template <class T>
void postOrder(BinaryTreeNode<T> * t, void (* visit) (BinaryTreeNode<T> *))
{
    if (t != NULL)
    {
        postOrder(t->leftChild, visit);
        postOrder(t->rightChild, visit);
        visit(t);
    }
}

template <class T>
void levelOrder(BinaryTreeNode<T> * t, void (* visit) (BinaryTreeNode<T> *))
{
    std::list<BinaryTreeNode<T> *> l;
    while (t != NULL)
    {
        visit(t);

        if (t->leftChild != NULL)
            l.push_back(t->leftChild);
        if (t->rightChild != NULL)
            l.push_back(t->rightChild);

        if (l.empty())
            return;

        t = l.front();
        l.pop_front();
    }
}

#endif//__BINARY_TREE_H__
