#include <iostream>
#include "SkipList.h"
 
using namespace std;

void Skiplist::bindNewNode(Node * x, Node * p) {
    if (!x->next) {
        x->next = p;
        p->prev = x;
    }
    else {
        p->next = x->next;
        x->next->prev = p;
        p->prev = x;
        x->next = p;
    }
}

void Skiplist::insert(int key) {
    Node * p = new Node;
    p->key = key;

    Node * x = head;
    while (1) { //find the prev node of p, which represents the right insert place
        if (x->key <= key) {
            if (x->next)
                x = x->next;
            else if (x->down)
                x = x->down;
            else break;
        }
        else if (x->prev->down)
            x = x->prev->down;
        else {
            x = x->prev;
            break;
        }
    }
    bindNewNode(x, p);
    while (rand() % 2) {  //throw the coin, then judge whether it needs to be higher according to the results
        Node * highp = new Node;
        highp->key = key;
        while (!x->top && x->prev)
            x = x->prev;
        if (x->top) {
            x = x->top;
            bindNewNode(x, highp);
            highp->down = p;
            p->top = highp;
        }
        else { //already the top, add a sentry
            Node * top = new Node;
            x = top;
            top->key = INT_MIN;
            top->down = head;
            head->top = top;
            head = top;
            bindNewNode(top, highp);
            highp->down = p;
            p->top = highp;
            ++level;
        }
        p = highp;
    }
    ++size;
}

void Skiplist::delNode(Node * x) {
    if (!x->next) { //node x is the last one
        if (x->prev == head && head->down) { //x is not at the bottom and x is the last one of this level
            head = head->down;
            head->top = nullptr;
            delete x->prev;
            --level;
        }
        else
            x->prev->next = nullptr;
    }
    else {
        x->prev->next = x->next;
        x->next->prev = x->prev;
    }
    delete x;
}

void Skiplist::remove(int key) {
    Node * x = searchNode(key);
    if (x) {
        while (x->down) {
            Node * y = x->down;
            delNode(x);
            x = y;
        }
        delNode(x);
        --size;
    }
}

Skiplist::Node * Skiplist::searchNode(int key) {
    Node * x = head;
    while (1) { //find the prev node of p, which represents the right insert place
        if (x->key == key)
            return x;
        else if (x->key < key) {
            if (x->next)
                x = x->next;
            else if (x->down)
                x = x->down;
            else
                return nullptr;
        }
        else if (x->prev->down)
            x = x->prev->down;
        else {
            return nullptr;
        }
    }
}

void Skiplist::showSkiplist() {
    Node * x = head, * y = head;
    while (y) {
        x = y;
        while (x) {
            if (x->prev != nullptr)
                cout << x->key << ' ';
            x = x->next;
        }
        cout << endl;
        y = y->down;
    }
}