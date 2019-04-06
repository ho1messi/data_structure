#ifndef __BINARY_SEARCH_TREE_H__
#define __BINARY_SEARCH_TREE_H__

#include <cstddef>
#include <utility>
#include <list>

template <class K, class V>
class BinarySearchTree
{
public:

    struct Node
    {
        typedef std::pair<const K, V> elem_type;
        typedef elem_type* elem_ptr;
        typedef size_t size_type;

        elem_type element;
        Node *leftChild, *rightChild;

        Node(const elem_type & element)
            : element(element), leftChild(NULL), rightChild(NULL) {}

        Node(const elem_type & element, Node * leftChild, Node * rightChild)
            : element(element), leftChild(leftChild), rightChild(rightChild) {}
    };

public:

    typedef Node node_type;

    typedef node_type* node_ptr;

    typedef typename node_type::elem_type elem_type;

    typedef elem_type* elem_ptr;

    typedef typename node_type::size_type size_type;

public:

    BinarySearchTree();

    ~BinarySearchTree();

    size_type height() const;

    bool empty() const;

    void clear();

    elem_ptr find(const K &) const;

    void insert(const K &, const V &);

    void erase(const K &);

    void preOrder(void (*) (node_ptr));

    void inOrder(void (*) (node_ptr));

    void postOrder(void (*) (node_ptr));

    void levelOrder(void (*) (node_ptr));

protected:

    void findParent(const K &, node_ptr &, node_ptr &);

    node_ptr replaceElement(node_ptr, node_ptr, const elem_type &);

    void eraseWithOneChild(node_ptr, node_ptr);

    static size_type heightRecursion(node_ptr);

    static node_ptr findLargest(node_ptr, node_ptr &);

    static node_ptr findSmallest(node_ptr, node_ptr &);

    static void preOrderRecursion(node_ptr, void (*) (node_ptr));

    static void inOrderRecursion(node_ptr, void (*) (node_ptr));

    static void postOrderRecursion(node_ptr, void (*) (node_ptr));

protected:

    node_ptr mRoot;

    size_type mTreeSize;
};


template <class K, class V>
BinarySearchTree<K, V>::BinarySearchTree()
    : mRoot(NULL), mTreeSize(0)
{

}

template <class K, class V>
BinarySearchTree<K, V>::~BinarySearchTree()
{
    clear();
}

template <class K, class V>
typename BinarySearchTree<K, V>::size_type
BinarySearchTree<K, V>::height() const
{
    return heightRecursion(this->mRoot);
}

template <class K, class V>
bool BinarySearchTree<K, V>::empty() const
{
    return mTreeSize == 0;
}

template <class K, class V>
void BinarySearchTree<K, V>::clear()
{
    postOrder([](node_ptr t){delete t;});
}

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
void BinarySearchTree<K, V>::insert(const K & key, const V & value)
{
    node_ptr p = this->mRoot, q = NULL;

    findParent(key, p, q);

    if (p != NULL)
        p->element.second = value;
    else
    {
        node_ptr t = new node_type(elem_type(key, value));
        if (this->mRoot != NULL)
            if (key < q->element.first)
                q->leftChild = t;
            else
                q->rightChild = t;
        else
            this->mRoot = t;

        this->mTreeSize++;
    }
}

template <class K, class V>
void BinarySearchTree<K, V>::erase(const K & key)
{
    node_ptr t = this->mRoot, parent_t = NULL;

    findParent(key, t, parent_t);

    if (t == NULL)
        return;

    if (t->leftChild != NULL && t->rightChild != NULL)
    {
        node_ptr p = t->leftChild, parent_p = t;
        p = findLargest(p, parent_p);

        node_ptr c = replaceElement(t, parent_t, p->element);

        if (parent_p == t)
            parent_t = c;
        else
            parent_t = parent_p;

        delete t;
        t = p;
    }

    eraseWithOneChild(t, parent_t);
}

template <class K, class V>
void BinarySearchTree<K, V>::preOrder(void (* visit) (node_ptr))
{
    preOrderRecursion(this->mRoot, visit);
}

template <class K, class V>
void BinarySearchTree<K, V>::inOrder(void (* visit) (node_ptr))
{
    inOrderRecursion(this->mRoot, visit);
}

template <class K, class V>
void BinarySearchTree<K, V>::postOrder(void (* visit) (node_ptr))
{
    postOrderRecursion(this->mRoot, visit);
}

template <class K, class V>
void BinarySearchTree<K, V>::levelOrder(void (* visit) (node_ptr))
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
void BinarySearchTree<K, V>::findParent(
    const K & key,
    node_ptr & result,
    node_ptr & parent)
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
BinarySearchTree<K, V>::replaceElement(
    node_ptr t,
    node_ptr parent,
    const elem_type & element)
{
    node_ptr p = new node_type(element, t->leftChild, t->rightChild);

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

template <class K, class V>
typename BinarySearchTree<K, V>::size_type
BinarySearchTree<K, V>::heightRecursion(node_ptr t)
{    
    if (t == NULL)
        return 0;

    size_type l = heightRecursion(t->leftChild);
    size_type r = heightRecursion(t->rightChild);
    return std::max(l, r) + 1;

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
void BinarySearchTree<K, V>::preOrderRecursion(
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
void BinarySearchTree<K, V>::inOrderRecursion(
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
void BinarySearchTree<K, V>::postOrderRecursion(
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


#endif//__BINARY_SEARCH_TREE_H__
