#ifndef PQUEUE_H_
#define PQUEUE_H_

#include <GL/glut.h>
#include <iostream>

struct pqitem;

class PriorityQueueArray {
private:
    int n;
    int max;
    pqitem *p;
public:
    PriorityQueueArray();
    void push (pqitem val);
    bool empty();
    pqitem top();
    void pop();
    int size();
    void trav ();
    
};

#endif
