#ifndef __AVL_TREE_H__
#define __AVL_TREE_H__

#include <cstddef>
#include <utility>
#include <algorithm>
#include <list>

template <class K, class V>
class AVLTree
{
public:

    struct Node
    {
        typedef std::pair<const K, V> elem_type;
        typedef elem_type* elem_ptr;
        typedef size_t size_type;

        elem_type element;
        size_type height;
        Node *leftChild, *rightChild;

        Node(const elem_type & element)
            : element(element), height(1), leftChild(NULL), rightChild(NULL) {}

        Node(const elem_type & element, Node * leftChild, Node * rightChild)
            : element(element), height(1), 
            leftChild(leftChild), rightChild(rightChild) {}
    };

public:

    typedef Node node_type;

    typedef node_type* node_ptr;

    typedef typename node_type::elem_type elem_type;

    typedef elem_type* elem_ptr;

    typedef typename node_type::size_type size_type;

    typedef int bf_type;

public:

    AVLTree();

    size_type height() const;

    elem_ptr find(const K &) const;

    void insert(const K &, const V &);

    void erase(const K &);

    void preOrder(void (*) (node_ptr));

    void inOrder(void (*) (node_ptr));

    void postOrder(void (*) (node_ptr));

    void levelOrder(void (*) (node_ptr));

private:

    static node_ptr insertRecursion(node_ptr, const K &, const V &);

    static node_ptr eraseRecursion(node_ptr, const K &, bool & found);

    static node_ptr rebalance(node_ptr);

    static size_type updateHeight(node_ptr);

    static bf_type getBF(node_ptr);

    static size_type heightRecursion(node_ptr);

    static void preOrderRecursion(node_ptr, void (*) (node_ptr));

    static void inOrderRecursion(node_ptr, void (*) (node_ptr));

    static void postOrderRecursion(node_ptr, void (*) (node_ptr));

    static node_ptr rotateLL(node_ptr);

    static node_ptr rotateRR(node_ptr);

    static node_ptr rotateLR(node_ptr);

    static node_ptr rotateRL(node_ptr);

    static node_ptr insertToBottomRight(node_ptr, node_ptr);

protected:
    node_ptr mRoot;
    size_type mTreeSize;
};

template <class K, class V>
AVLTree<K, V>::AVLTree()
    : mRoot(NULL), mTreeSize(0)
{

}

template <class K, class V>
typename AVLTree<K, V>::size_type
AVLTree<K, V>::height() const
{
    return heightRecursion(this->mRoot);
}

template <class K, class V>
typename AVLTree<K, V>::elem_ptr
AVLTree<K, V>::find(const K & key) const
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
void AVLTree<K, V>::insert(const K & key, const V & value)
{
    if (this->mRoot == NULL)
        this->mRoot = new node_type(elem_type(key, value));
    else
        this->mRoot = insertRecursion(this->mRoot, key, value);

    this->mTreeSize++;

    rebalance(this->mRoot);
}

template <class K, class V>
void AVLTree<K, V>::erase(const K & key)
{
    if (this->mRoot == NULL)
        return;

    bool found = false;
    this->mRoot = eraseRecursion(this->mRoot, key, found);

    if (found)
        this->mTreeSize++;
}

template <class K, class V>
void AVLTree<K, V>::preOrder(void (* visit) (node_ptr))
{
    preOrderRecursion(this->mRoot, visit);
}

template <class K, class V>
void AVLTree<K, V>::inOrder(void (* visit) (node_ptr))
{
    inOrderRecursion(this->mRoot, visit);
}

template <class K, class V>
void AVLTree<K, V>::postOrder(void (* visit) (node_ptr))
{
    postOrderRecursion(this->mRoot, visit);
}

template <class K, class V>
void AVLTree<K, V>::levelOrder(void (* visit) (node_ptr))
{    
    std::list<node_ptr> l;
    node_ptr t = this->mRoot;

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

template <class K, class V>
typename AVLTree<K, V>::node_ptr
AVLTree<K, V>::insertRecursion(AVLTree<K, V>::node_ptr t, const K & key, const V & value)
{
    if (key < t->element.first)
        if (t->leftChild == NULL)
            t->leftChild = new node_type(elem_type(key, value));
        else
            t->leftChild = insertRecursion(t->leftChild, key, value);
    else
        if (t->rightChild == NULL)
            t->rightChild = new node_type(elem_type(key, value));
        else
            t->rightChild = insertRecursion(t->rightChild, key, value);

    return rebalance(t);
}

template <class K, class V>
typename AVLTree<K, V>::node_ptr
AVLTree<K, V>::eraseRecursion(AVLTree<K, V>::node_ptr t, const K & key, bool & found)
{
    node_ptr newRoot = t;

    if (key == t->element.first)
    {
        if (t->leftChild != NULL && t->rightChild != NULL)
            t->leftChild = insertToBottomRight(t->leftChild, t->rightChild);

        newRoot = t->leftChild;
        found = true;
        delete t;
    }
    else
    {
        if (key < t->element.first)
            t->leftChild = eraseRecursion(t->leftChild, key, found);
        else if (key > t->element.first)
            t->rightChild = eraseRecursion(t->rightChild, key, found);

        newRoot = rebalance(newRoot);
    }

    return newRoot;
}

template <class K, class V>
typename AVLTree<K, V>::node_ptr
AVLTree<K, V>::rebalance(AVLTree<K, V>::node_ptr t)
{
    updateHeight(t);

    if (getBF(t) == 2)
    {
        if (getBF(t->leftChild) == -1)
            return rotateLR(t);
        return rotateLL(t);
    }
    else if (getBF(t) == -2)
    {
        if (getBF(t->rightChild) == 1)
            return rotateRL(t);
        return rotateRR(t);
    }

    return t;
}

template <class K, class V>
typename AVLTree<K, V>::size_type
AVLTree<K, V>::updateHeight(node_ptr t)
{
    size_type l = heightRecursion(t->leftChild);
    size_type r = heightRecursion(t->rightChild);

    t->height = std::max(l, r) + 1;

    return t->height;
}

template <class K, class V>
typename AVLTree<K, V>::bf_type
AVLTree<K, V>::getBF(AVLTree::node_ptr t)
{
    return heightRecursion(t->leftChild) - heightRecursion(t->rightChild);
}

template <class K, class V>
typename AVLTree<K, V>::size_type
AVLTree<K, V>::heightRecursion(node_ptr t)
{
    if (t == NULL)
        return 0;

    return t->height;
}

template <class K, class V>
void AVLTree<K, V>::preOrderRecursion(node_ptr t, void (* visit) (node_ptr))
{
    if (t != NULL)
    {
        visit(t);
        preOrderRecursion(t->leftChild, visit);
        preOrderRecursion(t->rightChild, visit);
    }
}

template <class K, class V>
void AVLTree<K, V>::inOrderRecursion(node_ptr t, void (* visit) (node_ptr))
{
    if (t != NULL)
    {
        inOrderRecursion(t->leftChild, visit);
        visit(t);
        inOrderRecursion(t->rightChild, visit);
    }
}

template <class K, class V>
void AVLTree<K, V>::postOrderRecursion(node_ptr t, void (* visit) (node_ptr))
{
    if (t != NULL)
    {
        postOrderRecursion(t->leftChild, visit);
        postOrderRecursion(t->rightChild, visit);
        visit(t);
    }
}

template <class K, class V>
typename AVLTree<K, V>::node_ptr
AVLTree<K, V>::rotateLL(AVLTree<K, V>::node_ptr t)
{
    node_ptr newRoot = t->leftChild;
    t->leftChild = newRoot->rightChild;
    newRoot->rightChild = t;

    updateHeight(t);
    updateHeight(newRoot);

    return newRoot;
}

template <class K, class V>
typename AVLTree<K, V>::node_ptr
AVLTree<K, V>::rotateRR(AVLTree<K, V>::node_ptr t)
{
    node_ptr newRoot = t->rightChild;
    t->rightChild = newRoot->leftChild;
    newRoot->leftChild = t;

    updateHeight(t);
    updateHeight(newRoot);

    return newRoot;
}

template <class K, class V>
typename AVLTree<K, V>::node_ptr
AVLTree<K, V>::rotateLR(AVLTree<K, V>::node_ptr t)
{
    t->leftChild = rotateRR(t->leftChild);
    return rotateLL(t);
}

template <class K, class V>
typename AVLTree<K, V>::node_ptr
AVLTree<K, V>::rotateRL(AVLTree<K, V>::node_ptr t)
{
    t->rightChild = rotateLL(t->rightChild);
    return rotateRR(t);
}

template <class K, class V>
typename AVLTree<K, V>::node_ptr
AVLTree<K, V>::insertToBottomRight(
        AVLTree<K, V>::node_ptr t,
        AVLTree<K, V>::node_ptr n)
{
    if (t->rightChild == NULL)
        t->rightChild = n;
    else
        t->rightChild = insertToBottomRight(t->rightChild, n);

    return rebalance(t);
}

#endif//__AVL_TREE_H__
