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

    node_ptr replaceElement(node_ptr t, node_ptr parent, const elem_type & elem);

    void eraseWithOneChild(node_ptr t, node_ptr parent);
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
    typename BinarySearchTree<K, E>::node_ptr t = this->mRoot, parent = NULL;

    findParent(key, t, parent);

    if (t == NULL)
        return;

    if (t->leftChild != NULL && t->rightChild != NULL)
    {
        typename BinarySearchTree<K, E>::node_ptr p = t->leftChild, parent_r = t;
        p = findLargest(p, parent_r);

        typename BinarySearchTree<K, E>::node_ptr c = 
            replaceElement(t, parent, p->element);

        if (parent_r == t)
            parent = c;
        else
            parent = parent_r;

        delete t;
        t = p;
    }

    eraseWithOneChild(t, parent);
}

template <class K, class E>
void BinarySearchTree<K, E>::findParent(
    const K & key,
    BinaryTreeNode<std::pair<const K, E> > * & result,
    BinaryTreeNode<std::pair<const K, E> > * & parent)
{
    result = this->mRoot;

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
    while (t->leftChild != NULL)
    {
        parent = t;
        t = t->leftChild;
    }

    return t;
}

template <class K, class E>
typename BinarySearchTree<K, E>::node_ptr
BinarySearchTree<K, E>::replaceElement(
    BinarySearchTree<K, E>::node_ptr t,
    BinarySearchTree<K, E>::node_ptr parent,
    const BinarySearchTree<K,E>::elem_type & elem)
{
    typename BinarySearchTree<K, E>::node_ptr p = new 
        BinarySearchTree<K, E>::node_type(elem, t->leftChild, t->rightChild);

    if (parent == NULL)
        this->mRoot = p;
    else if (t == parent->leftChild)
        parent->leftChild = p;
    else
        parent->rightChild = p;

    return p;
}

template <class K, class E>
void BinarySearchTree<K, E>::eraseWithOneChild(
    BinarySearchTree<K, E>::node_ptr t,
    BinarySearchTree<K, E>::node_ptr parent)
{
    typename BinarySearchTree<K, E>::node_ptr p =
        t->leftChild != NULL ? t->leftChild : t->rightChild;

    if (t == this->mRoot)
        this->mRoot = p;
    else
    {
        if (t == parent->leftChild)
            parent->leftChild = p;
        else
            parent->rightChild = p;
    }

    this->mTreeSize--;
    delete t;
}


#endif//__BINARY_SEARCH_TREE_H__
