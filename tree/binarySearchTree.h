#ifndef __BINARY_SEARCH_TREE_H__
#define __BINARY_SEARCH_TREE_H__

#include <utility>

#include "linkedBinaryTree.h"

template <class K, class V>
class BinarySearchTree : public LinkedBinaryTree<std::pair<const K, V> >
{
public:

    typedef BinaryTreeNode<std::pair<const K, V> > node_type;

    typedef BinaryTreeNode<std::pair<const K, V> >* node_ptr;

    typedef std::pair<const K, V> elem_type;

    typedef std::pair<const K, V>* elem_ptr;


    elem_ptr find(const K &) const;

    void insert(const elem_type &);

    void insert(const K &, const V &);

    void erase(const K &);

protected:

    void findParent(const K & key, node_ptr & result, node_ptr & parent);

    static node_ptr findLargest(node_ptr t, node_ptr & parent);

    static node_ptr findSmallest(node_ptr t, node_ptr & parent);

    node_ptr replaceElement(node_ptr t, node_ptr parent, const elem_type & elem);

    void eraseWithOneChild(node_ptr t, node_ptr parent);
};

template <class K, class V>
typename BinarySearchTree<K, V>::elem_ptr 
BinarySearchTree<K, V>::find(const K & key) const
{
    node_ptr p = this->mRoot;

    while (p != NULL)
    {
        if (key < p->element.first)
            p = p->leftChild;
        else if (key > p->element.first)
            p = p->rightChild;
        else
            return &p->element;
    }

    return NULL;
}

template <class K, class V>
void BinarySearchTree<K, V>::insert(
        const BinarySearchTree<K, V>::elem_type & v)
{
    node_ptr p = this->mRoot, q = NULL;

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

template <class K, class V>
void BinarySearchTree<K, V>::insert(const K & key, const V & elem)
{
    std::pair<const K, V> p(key, elem);
    insert(p);
}

template <class K, class V>
void BinarySearchTree<K, V>::erase(const K & key)
{
    node_ptr t = this->mRoot, parent = NULL;

    findParent(key, t, parent);

    if (t == NULL)
        return;

    if (t->leftChild != NULL && t->rightChild != NULL)
    {
        node_ptr p = t->leftChild, parent_r = t;
        p = findLargest(p, parent_r);

        node_ptr c = replaceElement(t, parent, p->element);

        if (parent_r == t)
            parent = c;
        else
            parent = parent_r;

        delete t;
        t = p;
    }

    eraseWithOneChild(t, parent);
}

template <class K, class V>
void BinarySearchTree<K, V>::findParent(
    const K & key,
    BinaryTreeNode<std::pair<const K, V> > * & result,
    BinaryTreeNode<std::pair<const K, V> > * & parent)
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

template <class K, class V>
typename BinarySearchTree<K, V>::node_ptr
BinarySearchTree<K, V>::findLargest(
    BinarySearchTree<K, V>::node_ptr t,
    BinarySearchTree<K, V>::node_ptr & parent)
{
    while (t->rightChild != NULL)
    {
        parent = t;
        t = t->rightChild;
    }

    return t;
}

template <class K, class V>
typename BinarySearchTree<K, V>::node_ptr
BinarySearchTree<K, V>::findSmallest(
    BinarySearchTree<K, V>::node_ptr t,
    BinarySearchTree<K, V>::node_ptr & parent)
{
    while (t->leftChild != NULL)
    {
        parent = t;
        t = t->leftChild;
    }

    return t;
}

template <class K, class V>
typename BinarySearchTree<K, V>::node_ptr
BinarySearchTree<K, V>::replaceElement(
    BinarySearchTree<K, V>::node_ptr t,
    BinarySearchTree<K, V>::node_ptr parent,
    const BinarySearchTree<K,V>::elem_type & elem)
{
    node_ptr p = new node_type(elem, t->leftChild, t->rightChild);

    if (parent == NULL)
        this->mRoot = p;
    else if (t == parent->leftChild)
        parent->leftChild = p;
    else
        parent->rightChild = p;

    return p;
}

template <class K, class V>
void BinarySearchTree<K, V>::eraseWithOneChild(
    BinarySearchTree<K, V>::node_ptr t,
    BinarySearchTree<K, V>::node_ptr parent)
{
    node_ptr p = t->leftChild != NULL ? t->leftChild : t->rightChild;

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
