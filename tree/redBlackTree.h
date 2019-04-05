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
        RED,
        BLACK
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
            : element(element), color(color), blackCount(1), 
            leftChild(NULL), rightChild(NULL) {}

        Node(const elem_type & element, NodeColor color,
                Node * leftChild, Node * rightChild)
            : element(element), color(color), blackCount(1), 
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

    void adjustRoot();

    static size_type heightRecursion(node_ptr);

    static node_ptr insertRecursion(const K &, const V &, node_ptr);

    static node_ptr insertAdjustRecursion(node_ptr);

    static color_type getColor(node_ptr);

    static size_type countColor(node_ptr, color_type);

    static node_ptr adjustChangeColor(node_ptr);

    static node_ptr adjustRotateLL(node_ptr);

    static node_ptr adjustRotateLR(node_ptr);

    static node_ptr adjustRotateRR(node_ptr);

    static node_ptr adjustRotateRL(node_ptr);

    static node_ptr rotateLeft(node_ptr);

    static node_ptr rotateRight(node_ptr);

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
    if (mRoot == NULL)
        mRoot = new node_type(elem_type(key, value), color_type::BLACK);
    else
        mRoot = insertRecursion(key, value, mRoot);

    adjustRoot();
}

template <class K, class V>
void RedBlackTree<K, V>::erase(const K & key)
{
    

    adjustRoot();
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
void RedBlackTree<K, V>::adjustRoot()
{
    if (getColor(mRoot) == color_type::RED)
        mRoot->color = color_type::BLACK;
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
RedBlackTree<K, V>::insertRecursion(const K & key, const V & value, node_ptr t)
{
    if (t == NULL)
        return new node_type(elem_type(key, value), color_type::RED);

    if (key < t->element.first)
        t->leftChild = insertRecursion(key, value, t->leftChild);
    else
        t->rightChild = insertRecursion(key, value, t->rightChild);

    return insertAdjustRecursion(t);
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::insertAdjustRecursion(node_ptr t)
{
    if (getColor(t) == color_type::RED)
        return t;

    if (countColor(t, color_type::RED) == 2)
    {
        if (countColor(t->leftChild, color_type::RED) == 1 ||
                countColor(t->rightChild, color_type::RED) == 1)
            return adjustChangeColor(t);
    }
    else if (countColor(t, color_type::RED) == 1)
    {
        if (getColor(t->leftChild) == color_type::RED)
        {
            if (getColor(t->leftChild->leftChild) == color_type::RED)
                return adjustRotateLL(t);
            else if (getColor(t->leftChild->rightChild) == color_type::RED)
                return adjustRotateLR(t);
        }
        else
        {
            if (getColor(t->rightChild->leftChild) == color_type::RED)
                return adjustRotateRL(t);
            else if (getColor(t->rightChild->rightChild) == color_type::RED)
                return adjustRotateRR(t);
        }
    }

    return t;
}

template <class K, class V>
typename RedBlackTree<K, V>::color_type
RedBlackTree<K, V>::getColor(node_ptr t)
{
    if (t == NULL)
        return color_type::BLACK;

    return t->color;
}

template <class K, class V>
typename RedBlackTree<K, V>::size_type
RedBlackTree<K, V>::countColor(node_ptr t, color_type color)
{
    size_type result = getColor(t->leftChild) == color ? 1 : 0;
    result += getColor(t->rightChild) == color ? 1 : 0;

    return result;
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::adjustChangeColor(node_ptr t)
{
    t->color = color_type::RED;
    t->leftChild->color = color_type::BLACK;
    t->rightChild->color = color_type::BLACK;

    return t;
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::adjustRotateLL(node_ptr t)
{
    t->color = color_type::RED;
    t->leftChild->color = color_type::BLACK;

    return rotateRight(t);
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::adjustRotateLR(node_ptr t)
{
    t->color = color_type::RED;
    t->leftChild->rightChild->color = color_type::BLACK;
    
    t->leftChild = rotateLeft(t->leftChild);
    return rotateRight(t);
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::adjustRotateRL(node_ptr t)
{
    t->color = color_type::RED;
    t->rightChild->leftChild->color = color_type::BLACK;
    
    t->rightChild = rotateRight(t->rightChild);
    return rotateLeft(t);
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::adjustRotateRR(node_ptr t)
{
    t->color = color_type::RED;
    t->rightChild->color = color_type::BLACK;
    return rotateLeft(t);
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::rotateLeft(node_ptr t)
{
    node_ptr newRoot = t->rightChild;
    t->rightChild = newRoot->leftChild;
    newRoot->leftChild = t;

    return newRoot;
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::rotateRight(node_ptr t)
{
    node_ptr newRoot = t->leftChild;
    t->leftChild = newRoot->rightChild;
    newRoot->rightChild = t;

    return newRoot;
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
