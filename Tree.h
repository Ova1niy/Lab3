//
// Created by Ova1niy on 07/06/2020.
//

#ifndef LAB3_TREE_H
#define LAB3_TREE_H

#include <iostream>
#include <list>
#include <queue>
#include <string>

using namespace std;

template <class T>
struct node {
    T key;
    unsigned char height;
    node* left;
    node* right;
    explicit node(T k) { key = k; left = right = 0; height = 1; }
};


template<class T>
class Tree {
private:
    node<T>* root;

    unsigned char height(node<T>* p);
    int bFactor(node<T>* p);
    void fixHeight(node<T>* p);
    node<T>* rotateRight(node<T>* p);
    node<T>* rotateLeft(node<T>* q);
    node<T>* balance(node<T>* p);

    node<T>* findMin(node<T>* p);
    node<T>* removeMin(node<T>* p);
    node<T>* remove(node<T>* p, T k);

    void destroyTree(node<T>* leaf);
    node<T>* insert(int key, node<T>* leaf);
    node<T>* search(int key, node<T>* leaf);

    void map(T (*operation)(T, T), T val, node<T>* leaf);
    void reduce(T (*operation)(T), node<T>* leaf, T* res);
    void where (bool (*filter)(T), Tree<T>* res);

    void visit (node<T>* leaf, string format, string* res);
    bool checkTree(node<T>* l1, node<T>* l2) const;
public:
    Tree();
    ~Tree();

    node<T>* balance();

    void remove(T key);

    void destroyTree();
    void insert(T key);
    node<T>* search(int key);

    void map(T (*operation)(T, T), T val);
    T reduce(T (*operation)(T));
    Tree<T>* where (bool (*filter)(T));
    Tree<T>* getSub(node<T>* slice);

    string getStr(string format);
    bool contains(Tree<T>* slice);
};

template<class T>
Tree<T>::Tree() {
    root = NULL;
}

template<class T>
Tree<T>::~Tree() {
    destroyTree();
}

template<class T>
unsigned char Tree<T>::height(node<T> *p) {
    return p? p->height : 0;
}

template<class T>
int Tree<T>::bFactor(node<T> *p) {
    return height(p->right) - height(p->left);
}

template<class T>
void Tree<T>::fixHeight(node<T> *p) {
    unsigned char hl = height(p->left);
    unsigned char hr = height(p->right);
    p->height = ((hl>hr)? hl : hr) + 1;
}

template<class T>
node<T> *Tree<T>::rotateRight(node<T> *p) {
    node<T>* q = p->left;
    p->left = q->right;
    q->right = p;
    fixHeight(p);
    fixHeight(q);
    return q;
}

template<class T>
node<T> *Tree<T>::rotateLeft(node<T> *q) {
    node<T>* p = q->right;
    q->right = p->left;
    p->left = q;
    fixHeight(q);
    fixHeight(p);
    return p;
}

template<class T>
node<T> *Tree<T>::balance() {
    return balance(root);
}


template<class T>
node<T> *Tree<T>::balance(node<T> *p) {
    fixHeight(p);
    if( bFactor(p) == 2 )
    {
        if( bFactor(p->right) < 0 )
            p->right = rotateRight(p->right);
        return rotateLeft(p);
    }
    if( bFactor(p) == -2 )
    {
        if( bFactor(p->left) > 0  )
            p->left = rotateLeft(p->left);
        return rotateRight(p);
    }
    return p;
}

template<class T>
void Tree<T>::destroyTree() {
    destroyTree(root);
}

template<class T>
void Tree<T>::destroyTree(node<T> *leaf) {
    if(leaf!=NULL)
    {
        destroy_tree(leaf->left);
        destroy_tree(leaf->right);
        delete leaf;
    }
}

template<class T>
void Tree<T>::insert(T key) {
    if (root != NULL) {
        root = insert(key, root);
//        fixHeight(root);
//        root = balance(root);
    } else {
        root = new node<T>(key);
    }
}

template<class T>
node<T>* Tree<T>::insert(int key, node<T>* leaf) {
    if (!leaf) return new node<T>(key);
    if (key < leaf->key)
        leaf->left = insert(key, leaf->left);
    else
        leaf->right = insert(key, leaf->right);
    return balance(leaf);
}

template<class T>
node<T> *Tree<T>::search(int key) {
    return search(key, root);
}

template<class T>
node<T> *Tree<T>::search(int key, node<T> *leaf) {
    if (leaf != NULL) {
        if (key == leaf->key) {
            return leaf;
        }
        if (key < leaf->key) {
            return search(key, leaf->left);
        } else {
            return search(key, leaf->right);
        }
    } else {
        return NULL;
    }
}

template<class T>
void Tree<T>::map(T (*operation)(T, T), T val) {
    map(operation, val, root);
}

template<class T>
void Tree<T>::map(T (*operation)(T, T), T val, node<T> *leaf) {
    auto *q = new list<node<T>*>;
    q->push_back(root);
    while (q->size() != 0) {
        node<T> *tmp = q->front();
        q->pop_front();
        tmp->key = operation(tmp->key, val);
        if (tmp->left) {
            q->push_back(tmp->left);
        }
        if (tmp->right) {
            q->push_back(tmp->right);
        }
    }
    delete(q);
}

template<class T>
T Tree<T>::reduce(T (*operation)(T)) {
    T* res = new T(0);
    reduce(operation, root, res);
    return *res;
}

template<class T>
void Tree<T>::reduce(T (*operation)(T), node<T> *leaf, T* res) {
    auto *q = new list<node<T>*>;
    q->push_back(root);
    while (q->size() != 0) {
        node<T> *tmp = q->front();
        q->pop_front();
        *res += operation(tmp->key);
        if (tmp->left) {
            q->push_back(tmp->left);
        }
        if (tmp->right) {
            q->push_back(tmp->right);
        }
    }
    delete(q);
}

template<class T>
Tree<T> *Tree<T>::where(bool (*filter)(T)) {
    auto* res = new Tree<T>();
    where(filter, res);
    return res;
}

template<class T>
void Tree<T>::where(bool (*filter)(T), Tree<T> *res) {
    auto *q = new list<node<T>*>;
    q->push_back(root);
    while (q->size() != 0) {
        node<T> *tmp = q->front();
        q->pop_front();
        if (filter(tmp->key)) {
            res->insert(tmp->key);
        }
        if (tmp->left) {
            q->push_back(tmp->left);
        }
        if (tmp->right) {
            q->push_back(tmp->right);
        }
    }
    delete(q);
}

template<class T>
node<T> *Tree<T>::findMin(node<T> *p) {
    return p->left? findMin(p->left) : p;;
}

template<class T>
node<T> *Tree<T>::removeMin(node<T> *p) {
    if(p->left == 0)
        return p->right;
    p->left = removeMin(p->left);
    return balance(p);
}

template<class T>
void Tree<T>::remove(T key) {
    root = remove(root, key);
}

template<class T>
node<T> *Tree<T>::remove(node<T> *p, T k) {
    if(!p) {
        return 0;
    }
    if(k < p->key) {
        p->left = remove(p->left, k);
    } else if (k > p->key) {
        p->right = remove(p->right, k);
    } else {
        node<T>* q = p->left;
        node<T>* r = p->right;
        delete p;
        if( !r ) return q;
        node<T>* min = findMin(r);
        min->right = removeMin(r);
        min->left = q;
        return balance(min);
    }
    return balance(p);
}

template<class T>
Tree<T> *Tree<T>::getSub(node<T>* slice) {
    if (slice != NULL) {
        auto *res = new Tree<T>();
        auto *q = new list<node<T>*>;
        q->push_back(slice);
        while (q->size() != 0) {
            node<T> *tmp = q->front();
            q->pop_front();
            res->insert(tmp->key);
            if (tmp->left) {
                q->push_back(tmp->left);
            }
            if (tmp->right) {
                q->push_back(tmp->right);
            }
        }
        delete (q);
        return res;
    } else {
        throw "NoSuchKeyInTree";
    }
}

template<class T>
void Tree<T>::visit(node<T>* leaf, string format, string* res) {
    if (leaf != NULL) {
        for (int i = 0; i < 3; ++i) {
            if (format[i] == 'L') {
                visit(leaf->left, format, res);
            } else if (format[i] == 'R') {
                visit(leaf->right, format, res);
            } else {
                *res += to_string(leaf->key);
                *res += " ";
            }
        }
    }
}

template<class T>
string Tree<T>::getStr(string format) {
    if (format.size() == 3) {
        bool N = false;
        bool L = false;
        bool R = false;
        for (int i = 0; i < 3; ++i) {
            if (format[i] == 'N') {
                N = true;
            } else if (format[i] == 'L') {
                L = true;
            } else if (format[i] == 'R') {
                R = true;
            } else {
                throw "UnexpectedSymbolInFormat";
            }
        }
        if (N && L && R) {
            auto *res = new string;
            visit(root, format, res);
            return *res;
        } else {
            throw "BadFormat";
        }
    } else {
        throw "WrongFromatSize";
    }
}

template<class T>
bool Tree<T>::contains(Tree<T> *slice) {
    node<T>* sub = search(slice->root->key);
    bool ans;
    while (sub->key == slice->root->key) {
        ans = checkTree(sub, slice->root);
        if (ans) {
            return true;
        }
        sub = sub->right;
    }
    return false;
}

template <typename T>
bool Tree<T>::checkTree(node<T>* l1, node<T>* l2) const {
    if (l1->key == l2->key) {
        std::queue<const node<T>*> queueOfNodes1;
        queueOfNodes1.push(l2);
        std::queue<const node<T>*> queueOfNodes2;
        queueOfNodes2.push(l1);
        while (queueOfNodes1.size()) {
            const node<T>* copy1 = queueOfNodes1.front();
            const node<T>* copy2 = queueOfNodes2.front();
            if (copy1->key != copy2->key) {
                return false;
            }
            queueOfNodes1.pop();
            queueOfNodes2.pop();
            if (copy1->left && copy2->left) {
                queueOfNodes1.push(copy1->left);
                queueOfNodes2.push(copy2->left);
            }
            if (copy1->right && copy2->right) {
                queueOfNodes1.push(copy1->right);
                queueOfNodes2.push(copy2->right);
            }
        }
        return true;
    }
    if (l2->key < l1->key) {
        return checkTree(l1->left, l2);
    }
    else {
        return checkTree(l1->right, l2);
    }
}

#endif //LAB3_TREE_H
