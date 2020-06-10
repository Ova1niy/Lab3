#include <iostream>
#include "Tree.h"

using namespace std;

template<class T>
T Add(T a1, T a2) {
    return a1 + a2;
}

template<class T>
T Summ(T a1) {
    return a1;
}

template<class T>
bool Negative(T a1) {
    return a1 < 0;
}

int main() {
    auto* myTree = new Tree<int>();
    for (int i = 0; i < 5; ++i) {
        myTree->insert(i);
    }
    cout << myTree->getStr("NLR") << endl;
    auto* tmp = new Tree<int>();
    for (int i = 0; i < 3; ++i) {
        tmp->insert(i);
    }
    cout << tmp->getStr("NLR") << endl;
    cout << myTree->contains(tmp) << endl;
    cout << myTree->contains(myTree->getSub(myTree->search(3))) << endl;
    return 0;
}
