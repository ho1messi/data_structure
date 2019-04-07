#ifndef __B_TREE_H__
#define __B_TREE_H__

#include <list>
#include <cstddef>
#include <utility>

template <class K, class V, size_t N>
class BTree
{
public:

    struct Node
    {
        typedef std::pair<const K, V> elem_type;
        typedef elem_type* elem_ptr;
        typedef Node* node_ptr;
        typedef size_t size_type;

        elem_ptr elements[N];
        node_ptr children[N + 1];

        Node()
            : elements{NULL}, children{NULL} {}

        Node(elem_type element)
            : elements{new elem_type(element)}, 
            children{NULL} {}

        Node(elem_type element, node_ptr child1, node_ptr child2)
            : elements{new elem_type(element)}, children{child1, child2} {}
    };

    typedef Node node_type;
    typedef node_type* node_ptr;
    typedef typename node_type::elem_type elem_type;
    typedef elem_type* elem_ptr;
    typedef typename node_type::size_type size_type;

    struct ElemChild
    {
        typedef typename Node::elem_ptr elem_ptr;
        typedef typename Node::node_ptr node_ptr;

        elem_ptr elem;
        node_ptr node;
    };

public:

    BTree();

    ~BTree();

    size_type height() const;

    bool empty() const;

    elem_ptr find(const K &) const;

    void insert(const K &, const V &);

    void erase(const K &);

    void clear();

    void preOrder(void (*) (node_ptr));

    void inOrder(void (*) (elem_ptr));

    void postOrder(void (*) (node_ptr));

    void levelOrder(void (*) (node_ptr));

private:

    static size_type heightRecursion(node_ptr);

    static elem_ptr findRecursion(node_ptr, const K &);

    static ElemChild insertRecursion(node_ptr, const K &, const V &);

    static void eraseRecursion(node_ptr, const K &);

    static size_t countElements(node_ptr);

    static ElemChild insertToNode(node_ptr, const ElemChild &);

    static ElemChild splitNode(node_ptr, const ElemChild &);

    static void insertNotFull(node_ptr, const ElemChild &);

    static void eraseLeaf(node_ptr, elem_ptr);

    static void repairNode(node_ptr, size_t);

    static void borrowFromLeftBro(node_ptr, size_t);

    static void borrowFromRightBro(node_ptr, size_t);

    static void mergeNodes(node_ptr, size_t);

    static elem_ptr findLargest(node_ptr);

    static node_ptr findLeftBrother(node_ptr, node_ptr);

    static void preOrderRecursion(node_ptr, void (*) (node_ptr));

    static void inOrderRecursion(node_ptr, void (*) (elem_ptr));

    static void postOrderRecursion(node_ptr, void (*) (node_ptr));

protected:

    node_ptr mRoot;

    size_type mTreeSize;

};

template <class K, class V, size_t N>
BTree<K, V, N>::BTree()
    : mRoot(NULL), mTreeSize(0)
{

}

template <class K, class V, size_t N>
BTree<K, V, N>::~BTree()
{
    clear();
}

template <class K, class V, size_t N>
typename BTree<K, V, N>::size_type 
BTree<K, V, N>::height() const
{
    return heightRecursion(mRoot);
}

template <class K, class V, size_t N>
bool BTree<K, V, N>::empty() const
{
    return mTreeSize == 0;
}

template <class K, class V, size_t N>
typename BTree<K, V, N>::elem_ptr 
BTree<K, V, N>::find(const K & key) const
{
    return findRecursion(mRoot, key);
}

template <class K, class V, size_t N>
void BTree<K, V, N>::insert(const K & key, const V & value)
{
    if (mRoot == NULL)
        mRoot = new node_type(elem_type(key, value));
    else
    {
        ElemChild result = insertRecursion(mRoot, key, value);

        if (result.elem != NULL)
        {
            node_ptr newRoot = new node_type(*result.elem, mRoot, result.node);
            mRoot = newRoot;
        }
    }
}

template <class K, class V, size_t N>
void BTree<K, V, N>::erase(const K & key)
{
    eraseRecursion(mRoot, key);

    if (mRoot->elements[0] == NULL)
    {
        node_ptr newRoot = mRoot->children[0];
        delete mRoot;
        mRoot = newRoot;
    }
}

template <class K, class V, size_t N>
void BTree<K, V, N>::clear()
{
    postOrder([](node_ptr t){delete t;});
}


template <class K, class V, size_t N>
void BTree<K, V, N>::preOrder(void (* visit) (node_ptr))
{
    preOrderRecursion(mRoot, visit);
}

template <class K, class V, size_t N>
void BTree<K, V, N>::inOrder(void (* visit) (elem_ptr))
{
    inOrderRecursion(mRoot, visit);
}

template <class K, class V, size_t N>
void BTree<K, V, N>::postOrder(void (* visit) (node_ptr))
{
    postOrderRecursion(mRoot, visit);
}

template <class K, class V, size_t N>
void BTree<K, V, N>::levelOrder(void (* visit) (node_ptr))
{
    std::list<node_ptr> l;
    node_ptr t = this->mRoot;

    while (t != NULL)
    {
        visit(t);

        size_t index = 0;
        while (t->children[index] != NULL)
            l.push_back(t->children[index]);

        if (l.empty())
            return;

        t = l.front();
        l.pop_front();
    }
}

template <class K, class V, size_t N>
typename BTree<K, V, N>::size_type 
BTree<K, V, N>::heightRecursion(node_ptr t)
{
    if (t == NULL)
        return 0;

    return 1 + heightRecursion(t->children[0]);
}

template <class K, class V, size_t N>
typename BTree<K, V, N>::elem_ptr 
BTree<K, V, N>::findRecursion(node_ptr t, const K & key)
{
    if (t == NULL)
        return NULL;

    size_t index = 0;
    while (t->elements[index] != NULL && t->elements[index]->first < key)
        index++;

    if (t->elements[index] != NULL && t->elements[index]->first == key)
        return t->elements[index];

    return findRecursion(t->children[index], key);
}

template <class K, class V, size_t N>
typename BTree<K, V, N>::ElemChild
BTree<K, V, N>::insertRecursion(node_ptr t, const K & key, const V & value)
{
    if (t->children[0] == NULL)
        return insertToNode(t, ElemChild{new elem_type(key, value), NULL});

    size_t index = 0;
    while (t->elements[index] != NULL && t->elements[index]->first < key)
        index++;

    ElemChild result = insertRecursion(t->children[index], key, value);
    return insertToNode(t, result);
}

template <class K, class V, size_t N>
void BTree<K, V, N>::eraseRecursion(node_ptr t, const K & key)
{
    if (t == NULL)
        return;

    size_t index = 0;
    while (t->elements[index] != NULL && t->elements[index]->first < key)
        index++;

    if (t->elements[index] != NULL && t->elements[index]->first == key)
    {
        if (t->children[index] == NULL)
        {
            eraseLeaf(t, t->elements[index]);
            return;
        }
        else
        {
            elem_ptr leaf = findLargest(t->children[index]);
            t->elements[index] = leaf;
            eraseLeaf(t->children[index], leaf);
        }
    }
    else
        eraseRecursion(t->children[index], key);

    repairNode(t, index);
    return;
}

template <class K, class V, size_t N>
size_t BTree<K, V, N>::countElements(node_ptr t)
{
    size_t count = 0;
    while (t->elements[count] != NULL)
        count++;

    return count;
}

template <class K, class V, size_t N>
typename BTree<K, V, N>::ElemChild
BTree<K, V, N>::insertToNode(node_ptr t, const ElemChild & elemChild)
{
    if (elemChild.elem == NULL)
        return elemChild;

    size_t index = countElements(t);

    if (index < N - 1)
    {
        insertNotFull(t, elemChild);
        return ElemChild{NULL, NULL};
    }

    return splitNode(t, elemChild);
}

template <class K, class V, size_t N>
typename BTree<K, V, N>::ElemChild
BTree<K, V, N>::splitNode(node_ptr t, const ElemChild & elemChild)
{
    insertNotFull(t, elemChild);

    node_ptr newNode = new node_type(); 
    size_t d = (N + 1) / 2 - 1;
    ElemChild result = {t->elements[d], newNode};
    t->elements[d++] = NULL;

    size_t index = 0;
    while (index + d < N)
    {
        newNode->elements[index] = t->elements[index + d];
        newNode->children[index] = t->children[index + d];
        t->elements[index + d] = NULL;
        t->children[index + d] = NULL;
        index++;
    }
    newNode->children[index] = t->children[index + d];
    t->children[index + d] = NULL;

    return result;
}

template <class K, class V, size_t N>
void BTree<K, V, N>::insertNotFull(node_ptr t, const ElemChild & elemChild)
{
    size_t index = 0;
    while (t->elements[index] != NULL) index++;

    if (index > 0)
    {
        index--;
        while (index < N && t->elements[index]->first > elemChild.elem->first)
        {
            t->elements[index + 1] = t->elements[index];
            t->children[index + 2] = t->children[index + 1];
            index--;
        }
        index++;
    }

    t->elements[index] = elemChild.elem;
    t->children[index + 1] = elemChild.node;
}

template <class K, class V, size_t N>
void BTree<K, V, N>::eraseLeaf(node_ptr t, elem_ptr elem)
{
    if (t == NULL)
        return;

    size_t index = 0;
    while (t->elements[index] != NULL && t->elements[index]->first < elem->first)
        index++;

    if (t->elements[index] == NULL || t->elements[index]->first > elem->first)
    {
        eraseLeaf(t->children[index], elem);
        repairNode(t, index);
    }
    else
        while (t->elements[index++] != NULL)
            t->elements[index - 1] = t->elements[index];

}

template <class K, class V, size_t N>
void BTree<K, V, N>::repairNode(node_ptr t, size_t index)
{
    const size_t MIN_NUM = (N - 1) / 2;
    node_ptr x = t->children[index];

    if (countElements(x) >= MIN_NUM)
        return;

    node_ptr leftBro = index > 0 ? t->children[index - 1] : NULL;
    node_ptr rightBro = t->children[index + 1];

    if (leftBro != NULL && countElements(leftBro) > MIN_NUM)
        borrowFromLeftBro(t, index);
    else if (rightBro != NULL && countElements(rightBro) > MIN_NUM)
        borrowFromRightBro(t, index);
    else
    {
        mergeNodes(t, (leftBro == NULL ? index : index - 1));
    }
}

template <class K, class V, size_t N>
void BTree<K, V, N>::borrowFromLeftBro(node_ptr t, size_t index)
{
    node_ptr x = t->children[index];
    node_ptr left = t->children[index - 1];

    size_t indexL = countElements(left);
    size_t indexX = countElements(x);

    x->children[indexX + 1] = x->children[indexX];
    while (indexX > 0)
    {
        t->elements[indexX] = t->elements[indexX - 1];
        t->children[indexX] = t->children[indexX - 1];
        indexX--;
    }

    x->elements[0] = t->elements[index - 1];
    x->children[0] = left->children[indexL];
    t->elements[index - 1] = left->elements[indexL - 1];
    left->elements[indexL - 1] = NULL;
    left->children[indexL] = NULL;
}

template <class K, class V, size_t N>
void BTree<K, V, N>::borrowFromRightBro(node_ptr t, size_t index)
{
    node_ptr x = t->children[index];
    node_ptr right = t->children[index + 1];

    size_t indexR = 1;
    size_t indexX = countElements(x);

    x->elements[indexX] = t->elements[index];
    x->children[x->children[0] != NULL ? indexX + 1 : indexX] = right->children[0];
    t->elements[index] = right->elements[0];

    while (right->elements[indexR] != NULL)
    {
        right->elements[indexR - 1] = right->elements[indexR];
        right->children[indexR - 1] = right->children[indexR];
        indexR++;
    }
    right->children[indexR - 1] = right->children[indexR];

    right->elements[indexR - 1] = NULL;
    right->children[indexR] = NULL;
}

template <class K, class V, size_t N>
void BTree<K, V, N>::mergeNodes(node_ptr t, size_t index)
{
    node_ptr left = t->children[index];
    node_ptr right = t->children[index + 1];
    size_t indexL = countElements(left);
    
    left->elements[indexL++] = t->elements[index];
    while (t->elements[++index] != NULL)
    {
        t->elements[index - 1] = t->elements[index];
        t->children[index] = t->children[index + 1];
    }
    t->elements[index - 1] = NULL;
    t->children[index] = NULL;
    
    size_t indexR = 0;
    while (right->elements[indexR] != NULL)
    {
        left->elements[index] = right->elements[indexR];
        left->children[index++] = right->children[indexR++];
    }
    left->children[indexL] = right->children[indexR];

    delete right;
}

template <class K, class V, size_t N>
typename BTree<K, V, N>::elem_ptr 
BTree<K, V, N>::findLargest(node_ptr t)
{
    size_t index = countElements(t);

    if (index == 0)
        return NULL;

    if (t->children[index] != NULL)
        return findLargest(t->children[index]);
    else
        return t->elements[index - 1];
}

template <class K, class V, size_t N>
typename BTree<K, V, N>::node_ptr 
BTree<K, V, N>::findLeftBrother(node_ptr t, node_ptr parent)
{
    size_t index = 0;
    while (parent->children[index] != NULL && parent->children[index] != t)
        index++;

    if (index == 0 || parent->children[index] == NULL)
        return NULL;

    return parent->children[index - 1];
}

template <class K, class V, size_t N>
void BTree<K, V, N>::preOrderRecursion(node_ptr t, void (* visit) (node_ptr))
{
    if (t == NULL)
        return;

    visit(t);

    size_t index = 0;
    while (t->children[index] != NULL)
        preOrderRecursion(t->children[index++], visit);
}

template <class K, class V, size_t N>
void BTree<K, V, N>::inOrderRecursion(node_ptr t, void (* visit) (elem_ptr))
{
    if (t == NULL)
        return;

    size_t index = 0;
    while (t->elements[index] != NULL)
    {
        inOrderRecursion(t->children[index], visit);
        visit(t->elements[index++]);
    }
    inOrderRecursion(t->children[index], visit);
}

template <class K, class V, size_t N>
void BTree<K, V, N>::postOrderRecursion(node_ptr t, void (* visit) (node_ptr))
{
    if (t == NULL)
        return;

    size_t index = 0;
    while (t->children[index] != NULL)
        postOrderRecursion(t->children[index++], visit);

    visit(t);
}

#endif//__B_TREE_H__
