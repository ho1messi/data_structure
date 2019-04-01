#ifndef __LINKED_BINARY_TREE_H__
#define __LINKED_BINARY_TREE_H__

#include "binaryTree.h"

template <class E>
class LinkedBinaryTree : public BinaryTree<E>
{
public:
    typedef size_t size_type;

public:

    LinkedBinaryTree();

    ~LinkedBinaryTree();

    bool empty() const;

    size_type size() const;

    void erase();

    void preOrder(void (*) (BinaryTreeNode<E> *));

    void inOrder(void (*) (BinaryTreeNode<E> *));

    void postOrder(void (*) (BinaryTreeNode<E> *));

    void levelOrder(void (*) (BinaryTreeNode<E> *));

protected:

    BinaryTreeNode<E> * mRoot;

    size_type mTreeSize;
};

template <class E>
LinkedBinaryTree<E>::LinkedBinaryTree()
    : mRoot(NULL), mTreeSize(0)
{

}

template <class E>
LinkedBinaryTree<E>::~LinkedBinaryTree()
{
    erase();
}

template <class E>
bool LinkedBinaryTree<E>::empty() const
{
    return mTreeSize == 0;
}

template <class E>
typename LinkedBinaryTree<E>::size_type LinkedBinaryTree<E>::size() const
{
    return mTreeSize;
}

template <class E>
void LinkedBinaryTree<E>::erase()
{
    postOrder([](BinaryTreeNode<E> * t) {delete t;});
}

template <class E>
void LinkedBinaryTree<E>::preOrder(void (* visit) (BinaryTreeNode<E> *))
{
    ::preOrder(mRoot, visit);
}

template <class E>
void LinkedBinaryTree<E>::inOrder(void (* visit) (BinaryTreeNode<E> *))
{
    ::inOrder(mRoot, visit);
}

template <class E>
void LinkedBinaryTree<E>::postOrder(void (* visit) (BinaryTreeNode<E> *))
{
    ::postOrder(mRoot, visit);
}

template <class E>
void LinkedBinaryTree<E>::levelOrder(void (* visit) (BinaryTreeNode<E> *))
{
    ::levelOrder(mRoot, visit);
}


#endif//__LINKED_BINARY_TREE_H__
