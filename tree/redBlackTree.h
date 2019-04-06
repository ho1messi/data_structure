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

    ~RedBlackTree();

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

private:

    void adjustRoot();

    static color_type getColor(node_ptr);

    static size_type countColor(node_ptr, color_type);

    static size_type getBlackCount(node_ptr);

    static int getBlackFactor(node_ptr);

    static size_type heightRecursion(node_ptr);

    static size_type updateBlackCount(node_ptr);

    static node_ptr insertRecursion(node_ptr, const K &, const V &);

    static node_ptr eraseRecursion(node_ptr, const K &);

    static node_ptr eraseLargest(node_ptr, node_ptr & largest);

    static node_ptr insertAdjustRecursion(node_ptr);

    static node_ptr eraseAdjustRecursion(node_ptr);

    static node_ptr insertChangeColor(node_ptr);

    static node_ptr insertRotateLL(node_ptr);

    static node_ptr insertRotateLR(node_ptr);

    static node_ptr insertRotateRL(node_ptr);

    static node_ptr insertRotateRR(node_ptr);

    static node_ptr eraseChangeColorLb(node_ptr);

    static node_ptr eraseChangeColorRb(node_ptr);

    static node_ptr eraseRotateLb1(node_ptr);

    static node_ptr eraseRotateLb2(node_ptr);

    static node_ptr eraseRotateRb1(node_ptr);

    static node_ptr eraseRotateRb2(node_ptr);

    static node_ptr eraseRotateLr0(node_ptr);

    static node_ptr eraseRotateLr1(node_ptr);

    static node_ptr eraseRotateLr2(node_ptr);

    static node_ptr eraseRotateRr0(node_ptr);

    static node_ptr eraseRotateRr1(node_ptr);

    static node_ptr eraseRotateRr2(node_ptr);

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
RedBlackTree<K, V>::~RedBlackTree()
{
    clear();
}

template <class K, class V>
typename RedBlackTree<K, V>::size_type
RedBlackTree<K, V>::height() const
{
    return heightRecursion(this->mRoot);
}

template <class K, class V>
bool RedBlackTree<K, V>::empty() const
{
    return mTreeSize == 0;
}

template <class K, class V>
void RedBlackTree<K, V>::clear()
{
    postOrder([](node_ptr t){delete t;});
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
    mRoot = insertRecursion(mRoot, key, value);

    adjustRoot();
}

template <class K, class V>
void RedBlackTree<K, V>::erase(const K & key)
{
    mRoot = eraseRecursion(mRoot, key);

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
    if (t == NULL)
        return 0;

    size_type result = getColor(t->leftChild) == color ? 1 : 0;
    result += getColor(t->rightChild) == color ? 1 : 0;

    return result;
}

template <class K, class V>
typename RedBlackTree<K, V>::size_type
RedBlackTree<K, V>::getBlackCount(node_ptr t)
{
    if (t == NULL)
        return 0;

    return t->blackCount;
}

template <class K, class V>
int RedBlackTree<K, V>::getBlackFactor(node_ptr t)
{
    size_type l = getBlackCount(t->leftChild);
    size_type r = getBlackCount(t->rightChild);

    l += getColor(t->leftChild) == color_type::BLACK;
    r += getColor(t->rightChild) == color_type::BLACK;

    return l - r;
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
typename RedBlackTree<K, V>::size_type
RedBlackTree<K, V>::updateBlackCount(node_ptr t)
{
    size_type result = getBlackCount(t->leftChild);
    result += getColor(t->leftChild) == color_type::BLACK;

    t->blackCount = result;
    return result;
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::insertRecursion(node_ptr t, const K & key, const V & value)
{
    if (t == NULL)
        return new node_type(elem_type(key, value), color_type::RED);

    if (key < t->element.first)
        t->leftChild = insertRecursion(t->leftChild, key, value);
    else
        t->rightChild = insertRecursion(t->rightChild, key, value);

    return insertAdjustRecursion(t);
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::eraseRecursion(node_ptr t, const K & key)
{
    if (t == NULL)
        return t;

    if (key < t->element.first)
        t->leftChild = eraseRecursion(t->leftChild, key);
    else if (key > t->element.first)
        t->rightChild = eraseRecursion(t->rightChild, key);
    else if (t->leftChild == NULL)
    {
        node_ptr newRoot = t->rightChild;

        delete t;
        return newRoot;
    }
    else
    {
        node_ptr newRoot;
        t->leftChild = eraseLargest(t->leftChild, newRoot);
        newRoot->leftChild = t->leftChild;
        newRoot->rightChild = t->rightChild;

        delete t;
        t = newRoot;
    }

    return eraseAdjustRecursion(t);
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::eraseLargest(node_ptr t, node_ptr & largest)
{
    if (t == NULL)
        return largest = t;

    if (t->rightChild == NULL)
    {
        largest = t;
        return t->leftChild;
    }
    else
    {
        t->rightChild = eraseLargest(t->rightChild, largest);
        return eraseAdjustRecursion(t);
    }
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::insertAdjustRecursion(node_ptr t)
{
    updateBlackCount(t);

    if (getColor(t) == color_type::RED)
        return t;

    if (countColor(t, color_type::RED) == 2)
    {
        if (countColor(t->leftChild, color_type::RED) == 1 ||
                countColor(t->rightChild, color_type::RED) == 1)
            return insertChangeColor(t);
    }
    else if (countColor(t, color_type::RED) == 1)
    {
        if (getColor(t->leftChild) == color_type::RED)
        {
            if (getColor(t->leftChild->leftChild) == color_type::RED)
                return insertRotateLL(t);
            else if (getColor(t->leftChild->rightChild) == color_type::RED)
                return insertRotateLR(t);
        }
        else
        {
            if (getColor(t->rightChild->leftChild) == color_type::RED)
                return insertRotateRL(t);
            else if (getColor(t->rightChild->rightChild) == color_type::RED)
                return insertRotateRR(t);
        }
    }

    return t;
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::eraseAdjustRecursion(node_ptr t)
{
    if (getBlackFactor(t) == -1)
    {
        if (getColor(t->rightChild) == color_type::BLACK)
            if (countColor(t->rightChild, color_type::RED) == 0)
                return eraseChangeColorLb(t);
            else if (getColor(t->rightChild->leftChild) == color_type::BLACK)
                return eraseRotateLb1(t);
            else 
                return eraseRotateLb2(t);
        else
            if (countColor(t->rightChild->leftChild, color_type::RED) == 0)
                return eraseRotateLr0(t);
            else if (getColor(t->rightChild->leftChild->leftChild) == color_type::BLACK)
                return eraseRotateLr1(t);
            else
                return eraseRotateLr2(t);
    }
    else if (getBlackFactor(t) == 1)
    {
        if (getColor(t->leftChild) == color_type::BLACK)
            if (countColor(t->leftChild, color_type::RED) == 0)
                return eraseChangeColorRb(t);
            else if (getColor(t->leftChild->rightChild) == color_type::BLACK)
                return eraseRotateRb1(t);
            else 
                return eraseRotateRb2(t);
        else
            if (countColor(t->leftChild->rightChild, color_type::RED) == 0)
                return eraseRotateRr0(t);
            else if (getColor(t->leftChild->rightChild->rightChild) == color_type::BLACK)
                return eraseRotateRr1(t);
            else
                return eraseRotateRr2(t);
    }

    return t;
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::insertChangeColor(node_ptr t)
{
    t->color = color_type::RED;
    t->leftChild->color = color_type::BLACK;
    t->rightChild->color = color_type::BLACK;

    updateBlackCount(t);
    return t;
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::insertRotateLL(node_ptr t)
{
    t->color = color_type::RED;
    t->leftChild->color = color_type::BLACK;

    return rotateRight(t);
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::insertRotateLR(node_ptr t)
{
    t->color = color_type::RED;
    t->leftChild->rightChild->color = color_type::BLACK;
    
    t->leftChild = rotateLeft(t->leftChild);
    return rotateRight(t);
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::insertRotateRL(node_ptr t)
{
    t->color = color_type::RED;
    t->rightChild->leftChild->color = color_type::BLACK;
    
    t->rightChild = rotateRight(t->rightChild);
    return rotateLeft(t);
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::insertRotateRR(node_ptr t)
{
    t->color = color_type::RED;
    t->rightChild->color = color_type::BLACK;
    return rotateLeft(t);
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::eraseChangeColorLb(node_ptr t)
{
    t->color = color_type::BLACK;
    t->rightChild->color = color_type::RED;

    updateBlackCount(t);
    return t;
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::eraseChangeColorRb(node_ptr t)
{
    t->color = color_type::BLACK;
    t->leftChild->color = color_type::RED;

    updateBlackCount(t);
    return t;
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::eraseRotateLb1(node_ptr t)
{
    t->rightChild->color = t->color;
    t->color = color_type::BLACK;
    t->rightChild->rightChild->color = color_type::BLACK;

    node_ptr newRoot = rotateLeft(t);
    updateBlackCount(t);
    updateBlackCount(newRoot);

    return newRoot;
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::eraseRotateLb2(node_ptr t)
{
    t->rightChild->leftChild->color = t->color;
    t->color = color_type::BLACK;

    t->rightChild = rotateRight(t->rightChild);
    node_ptr newRoot = rotateLeft(t);
    updateBlackCount(t);
    updateBlackCount(newRoot);

    return newRoot;
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::eraseRotateRb1(node_ptr t)
{
    t->leftChild->color = t->color;
    t->color = color_type::BLACK;
    t->leftChild->leftChild->color = color_type::BLACK;

    node_ptr newRoot = rotateRight(t);
    updateBlackCount(t);
    updateBlackCount(newRoot);

    return newRoot;
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::eraseRotateRb2(node_ptr t)
{
    t->leftChild->rightChild->color = t->color;
    t->color = color_type::BLACK;

    t->leftChild = rotateLeft(t->leftChild);
    node_ptr newRoot = rotateRight(t);
    updateBlackCount(t);
    updateBlackCount(newRoot);

    return newRoot;
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::eraseRotateLr0(node_ptr t)
{
    t->rightChild->color = color_type::BLACK;
    t->rightChild->leftChild->color = color_type::RED;

    node_ptr newRoot = rotateLeft(t);
    updateBlackCount(t);

    return newRoot;
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::eraseRotateLr1(node_ptr t)
{
    t->rightChild->leftChild->rightChild->color = color_type::BLACK;

    t->rightChild = rotateRight(t->rightChild);
    node_ptr newRoot = rotateLeft(t);

    updateBlackCount(t);
    updateBlackCount(newRoot);

    return newRoot;
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::eraseRotateLr2(node_ptr t)
{
    t->rightChild->leftChild->leftChild->color = color_type::BLACK;

    t->rightChild->leftChild = rotateRight(t->rightChild->leftChild);
    t->rightChild = rotateRight(t->rightChild);
    node_ptr newRoot = rotateLeft(t);

    updateBlackCount(t);
    updateBlackCount(newRoot);

    return newRoot;
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::eraseRotateRr0(node_ptr t)
{
    t->leftChild->color = color_type::BLACK;
    t->leftChild->rightChild->color = color_type::RED;

    node_ptr newRoot = rotateRight(t);
    updateBlackCount(t);

    return newRoot;
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::eraseRotateRr1(node_ptr t)
{
    t->leftChild->rightChild->leftChild->color = color_type::BLACK;

    t->leftChild = rotateLeft(t->leftChild);
    node_ptr newRoot = rotateRight(t);

    updateBlackCount(t);
    updateBlackCount(newRoot);

    return newRoot;
}

template <class K, class V>
typename RedBlackTree<K, V>::node_ptr
RedBlackTree<K, V>::eraseRotateRr2(node_ptr t)
{
    t->leftChild->rightChild->rightChild->color = color_type::BLACK;

    t->leftChild->rightChild = rotateRight(t->leftChild->rightChild);
    t->leftChild = rotateLeft(t->leftChild);
    node_ptr newRoot = rotateRight(t);

    updateBlackCount(t);
    updateBlackCount(newRoot);

    return newRoot;
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
