#ifndef __SKIPLIST_H__
#define __SKIPLIST_H__
 
#include <climits>
#include <cstdlib>
#include <ctime>
#include <cmath>
 
class Skiplist {
private :
    struct Node {
        int key;
        Node * prev;
        Node * next;
        Node * down;
        Node * top;
        Node() : key(0), prev(nullptr), next(nullptr), down(nullptr), top(nullptr) {}
    };
private :
    Node * head;
    int level;
    int size;
private :
    void bindNewNode(Node * x, Node * p);
    void delNode(Node * x);
    Node * searchNode(int key);
public :
    Skiplist() : head(new Node), level(1),size(0)
        {head->key = INT_MIN; srand(static_cast<int>(time(0)));}
    ~Skiplist() {delete head;}
    void insert(int key);
    void remove(int key);
    bool search(int key) {return (searchNode(key) != nullptr);}
    void showSkiplist();
    int getLevel() {return level;}
    int getSize() {return size;}
};
 
#endif // __SKIPLIST_H__