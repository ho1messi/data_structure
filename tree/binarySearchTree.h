#ifndef __BINARY_SEARCH_TREE_H__
#define __BINARY_SEARCH_TREE_H__

#include <utility>

#include "linkedBinaryTree.h"

template <class K, class E>
class BinarySearchTree : public LinkedBinaryTree<std::pair<const K, E> >
{
public:

    typedef BinaryTreeNode<std::pair<const K, E> > node_type;

    typedef BinaryTreeNode<std::pair<const K, E> >* node_ptr;

    typedef std::pair<const K, E> elem_type;

    typedef std::pair<const K, E>* elem_ptr;


    elem_ptr find(const K &) const;

    void insert(const elem_type &);

    void insert(const K &, const E &);

    void erase(const K &);

protected:

    void findParent(const K & key, node_ptr & result, node_ptr & parent);

    static node_ptr findLargest(node_ptr t, node_ptr & parent);

    static node_ptr findSmallest(node_ptr t, node_ptr & parent);
};

template <class K, class E>
typename BinarySearchTree<K, E>::elem_ptr 
BinarySearchTree<K, E>::find(const K & key) const
{
    typename BinarySearchTree<K, E>::elem_ptr p = this->mRoot;

    while (p != NULL)
    {
        if (key < p->element.first)
            p = p->leftChild;
        else if (key > p->element.first)
            p = p->rightChild;
        else
            return &p->element;
    }
}

template <class K, class E>
void BinarySearchTree<K, E>::insert(
        const BinarySearchTree<K, E>::elem_type & v)
{
    typename BinarySearchTree<K, E>::node_ptr p = this->mRoot, q = NULL;

    findParent(v.first, p, q);

    if (p != NULL)
        p->element.second = v.second;
    else
    {
        p = new node_type(v);
        if (this->mRoot != NULL)
            if (v.first < q->element.first)
                q->leftChild = p;
            else
                q->rightChild = p;
        else
            this->mRoot = p;

        this->mTreeSize++;
    }
}

template <class K, class E>
void BinarySearchTree<K, E>::insert(const K & key, const E & elem)
{
    std::pair<const K, E> p(key, elem);
    insert(p);
}

template <class K, class E>
void BinarySearchTree<K, E>::erase(const K & key)
{
    typename BinarySearchTree<K, E>::node_ptr p = this->mRoot, q = NULL;

    findParent(key, p, q);

    if (p == NULL)
        return;

    if (p->leftChild != NULL && p->rightChild != NULL)
    {
        typename BinarySearchTree<K, E>::node_ptr r = p->leftChild, s = p;
        r = findLargest(r, s);

        typename BinarySearchTree<K, E>::node_ptr t 
            = new node_type(r->element, p->leftChild, p->rightChild);
        if (q == NULL)
            this->mRoot = t;
        else if (p == q->leftChild)
            q->leftChild = t;
        else
            q->rightChild = t;

        if (s == p)
            q = t;
        else
            q = s;

        delete p;
        p = r;
    }

    typename BinarySearchTree<K, E>::node_ptr r;
    if (p->leftChild != NULL)
        r = p->leftChild;
    else
        r = p->rightChild;

    if (p == this->mRoot)
        this->mRoot = r;
    else
    {
        if (p == q->leftChild)
            q->leftChild = r;
        else
            q->rightChild = r;
    }       

    this->mTreeSize--;
    delete p;
}

template <class K, class E>
void BinarySearchTree<K, E>::findParent(
    const K & key,
    BinaryTreeNode<std::pair<const K, E> > * & result,
    BinaryTreeNode<std::pair<const K, E> > * & parent)
{
    result = this->mRoot;
    parent = NULL;

    while (result != NULL && result->element.first != key)
    {
        parent = result;

        if (key < result->element.first)
            result = result->leftChild;
        else 
            result = result->rightChild;
    }
}

template <class K, class E>
typename BinarySearchTree<K, E>::node_ptr
BinarySearchTree<K, E>::findLargest(
    BinarySearchTree<K, E>::node_ptr t,
    BinarySearchTree<K, E>::node_ptr & parent)
{
    parent = NULL;

    while (t->rightChild != NULL)
    {
        parent = t;
        t = t->rightChild;
    }

    return t;
}

template <class K, class E>
typename BinarySearchTree<K, E>::node_ptr
BinarySearchTree<K, E>::findSmallest(
    BinarySearchTree<K, E>::node_ptr t,
    BinarySearchTree<K, E>::node_ptr & parent)
{
    parent = NULL;

    while (t->leftChild != NULL)
    {
        parent = t;
        t = t->leftChild;
    }

    return t;
}


#endif//__BINARY_SEARCH_TREE_H__
