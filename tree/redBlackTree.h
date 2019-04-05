#ifndef __RED_BLACK_TREE_H__
#define __RED_BLACK_TREE_H__

#include <utility>
#include <cstddef>
#include <algorithm>
#include <list>

template <class K, class V>
class RedBlackTree
{
public:

    enum NodeColor
    {
        Red,
        Black
    };

    struct Node
    {
        typedef std::pair<const K, V> elem_type;
        typedef elem_type* elem_ptr;
        typedef size_t size_type;

        elem_type element;
        NodeColor color;
        size_type blackCount;
        Node *leftChild, *rightChild;

        Node(const elem_type & element, NodeColor color)
            : element(0), color(color), blackCount(1), 
            leftChild(NULL), rightChild(NULL) {}

        Node(const elem_type & element, NodeColor color,
                Node * leftChild, Node * rightChild)
            : element(0), color(color), blackCount(1), 
            leftChild(leftChild), rightChild(rightChild) {}
    };

public:
    
    typedef Node node_type;

    typedef node_type* node_ptr;

    typedef typename node_type::elem_type elem_type;

    typedef elem_type* elem_ptr;

    typedef typename node_type::size_type size_type;

    typedef NodeColor color_type;

public:

    RedBlackTree();

    size_type height() const;

    elem_ptr find(const K &) const;

    void insert(const K &, const V &);

    void erase(const K &);

    void preOrder(void (*) (node_ptr));

    void inOrder(void (*) (node_ptr));

    void postOrder(void (*) (node_ptr));

    void levelOrder(void (*) (node_ptr));

private:

    static size_type heightRecursion(node_ptr);

    static node_ptr insertRecursion(const K &, const V &, node_ptr, node_ptr);

    static node_ptr adjustRecursion(node_ptr, node_ptr);

    static color_type getColor(node_ptr);

    static void preOrderRecursion(node_ptr, void (*) (node_ptr));

    static void inOrderRecursion(node_ptr, void (*) (node_ptr));

    static void postOrderRecursion(node_ptr, void (*) (node_ptr));

protected:

    node_ptr mRoot;

    size_type mTreeSize;
};

template <class K, class V>
RedBlackTree<K, V>::RedBlackTree()
    : mRoot(NULL), mTreeSize(0)
{

}

template <class K, class V>
typename RedBlackTree<K, V>::size_type
RedBlackTree<K, V>::height() const
{
    return heightRecursion(this->mRoot);
}

template <class K, class V>
typename RedBlackTree<K, V>::elem_ptr
RedBlackTree<K, V>::find(const K & key) const
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
void RedBlackTree<K, V>::insert(const K & key, const V & value)
{

}

template <class K, class V>
void RedBlackTree<K, V>::erase(const K & key)
{
    
}

template <class K, class V>
void RedBlackTree<K, V>::preOrder(void (* visit) (node_ptr))
{
    preOrderRecursion(this->mRoot, visit);
}

template <class K, class V>
void RedBlackTree<K, V>::inOrder(void (* visit) (node_ptr))
{
    inOrderRecursion(this->mRoot, visit);
}

template <class K, class V>
void RedBlackTree<K, V>::postOrder(void (* visit) (node_ptr))
{
    postOrderRecursion(this->mRoot, visit);
}

template <class K, class V>
void RedBlackTree<K, V>::levelOrder(void (* visit) (node_ptr))
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
typename RedBlackTree<K, V>::size_type
RedBlackTree<K, V>::heightRecursion(node_ptr t)
{
    if (t == NULL)
        return 0;

    size_type l = heightRecursion(t->leftChild);
    size_type r = heightRecursion(t->rightChild);
    return std::max(l, r) + 1;
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::insertRecursion(
        const K & key, const V & value, 
        node_ptr t, node_ptr parent)
{
    if (key < t->element.first)
        if (t->leftChild == NULL)
            t->leftChild = new node_type(key, value);
        else
            insertRecursion(key, t->leftChild, t);
    else
        if (t->rightChild == NULL)
            t->rightChild = new node_type(key, value);
        else
            insertRecursion(key, t->rightChild, t);

    return adjustRecursion(t, parent);
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::adjustRecursion(node_ptr t, node_ptr parent)
{
    if (getColor(t) == color_type::Red)
        if (getColor(t->leftChild) == color_type::Red)
}

template <class K, class V>
typename RedBlackTree<K, V>::color_type
RedBlackTree<K, V>::getColor(node_ptr t)
{
    if (t == NULL)
        return color_type::Black;

    return t->color;
}

template <class K, class V>
void RedBlackTree<K, V>::preOrderRecursion(
        node_ptr t,
        void (* visit) (node_ptr))
{
    if (t != NULL)
    {
        visit(t);
        preOrderRecursion(t->leftChild, visit);
        preOrderRecursion(t->rightChild, visit);
    }
}

template <class K, class V>
void RedBlackTree<K, V>::inOrderRecursion(
        node_ptr t,
        void (* visit) (node_ptr))
{
    if (t != NULL)
    {
        inOrderRecursion(t->leftChild, visit);
        visit(t);
        inOrderRecursion(t->rightChild, visit);
    }
}

template <class K, class V>
void RedBlackTree<K, V>::postOrderRecursion(
        node_ptr t,
        void (* visit) (node_ptr))
{
    if (t != NULL)
    {
        postOrderRecursion(t->leftChild, visit);
        postOrderRecursion(t->rightChild, visit);
        visit(t);
    }
}

#endif//__RED_BLACK_TREE_H__
