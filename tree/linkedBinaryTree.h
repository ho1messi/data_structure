#ifndef __LINKED_BINARY_TREE_H__
#define __LINKED_BINARY_TREE_H__

#include "binaryTree.h"

template <class E>
class LinkedBinaryTree : public BinaryTree<E>
{
    typedef typename BinaryTree<E>::size_type size_type;

    typedef BinaryTreeNode<E> node_type;
    typedef BinaryTreeNode<E>* node_ptr;

public:

    LinkedBinaryTree();

    ~LinkedBinaryTree();

    bool empty() const;

    size_type size() const;

    size_type height() const;

    void erase();

    void preOrder(void (*) (node_ptr));

    void inOrder(void (*) (node_ptr));

    void postOrder(void (*) (node_ptr));

    void levelOrder(void (*) (node_ptr));

protected:

    void insertLeft(node_ptr, node_ptr);
    void insertLeft(node_ptr, const E &);

    void insertRight(node_ptr, node_ptr);
    void insertRight(node_ptr, const E &);

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
typename LinkedBinaryTree<E>::size_type 
LinkedBinaryTree<E>::size() const
{
    return mTreeSize;
}

template <class E>
typename LinkedBinaryTree<E>::size_type
LinkedBinaryTree<E>::height() const
{
    return ::height(this->mRoot);
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

template <class E>
void LinkedBinaryTree<E>::insertLeft(
        BinaryTreeNode<E> * t,
        BinaryTreeNode<E> * p)
{
    if (t->leftChild == NULL)
        t->leftChild = p;
}

template <class E>
void LinkedBinaryTree<E>::insertLeft(BinaryTreeNode<E> * t, const E & elem)
{
    node_ptr p = new node_type(elem, NULL, NULL);
    insertLeft(t, p);
}

template <class E>
void LinkedBinaryTree<E>::insertRight(
        BinaryTreeNode<E> * t,
        BinaryTreeNode<E> * p)
{
    if (t->rightChild == NULL)
        t->rightChild = p;
}

template <class E>
void LinkedBinaryTree<E>::insertRight(BinaryTreeNode<E> * t, const E & elem)
{
    node_ptr p = new node_type(elem, NULL, NULL);
    insertRight(t, p);
}


#endif//__LINKED_BINARY_TREE_H__
