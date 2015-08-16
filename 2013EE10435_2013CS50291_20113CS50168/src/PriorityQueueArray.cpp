#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include "Structures.h"
#include "PriorityQueueArray.h"

PriorityQueueArray::PriorityQueueArray()
{
    n = 0;
    max = 10;
    p = new pqitem[max];
}


void PriorityQueueArray::push ( pqitem val )        //Pushing to PQueue
{
    
    if ( n == 0 ) {
        p[1] = val;
        n++;
    }
    else {
        n++;
        if ( n >= max ) {
            int j;
            pqitem *t;
            t = new pqitem[max*2];
            for ( j = 1; j < max; j++ ) {
                t[j] = p[j];
            }
            p = t;
            max = max*2;
        }
        int i = n;
        while (( p[i/2].tcol > val.tcol ) && i != 1 ) {
            p[i] = p[i/2];
            i = i/2;
        }
        p[i] = val;
    }
}

bool PriorityQueueArray::empty()            //Checking if empty
{
    return n == 0;
}

pqitem PriorityQueueArray::top()            //Assigning Top
{
    return p[1];
}

void PriorityQueueArray::pop()              //Popping from PQueue
{
    if ( n == 1 )
        n--;
    else {
        int i = 1;
        pqitem val = p[n];
        n--;
        int c;
        while ( 1 ) {
            c = 1;
            if ( 2*i == n ) {
                if ( val.tcol > p[2*i].tcol ) {
                    p[i] = p[2*i];
                    i = 2*i;
                }
            }
            else if ( 2*i > n ) {}
            else {
                int j = ( p[2*i].tcol > p[2*i+1].tcol )? 2*i+1 : 2*i;
                if ( val.tcol > p[j].tcol  ) {
                    p[i] = p[j];
                    i = j;
                    c = 0;
                }
            }
            if ( c == 1 ) {
                p[i] = val;
                break;
            }
        }
    }
}

int PriorityQueueArray::size()              //Returning size
{
    return n;
}

void PriorityQueueArray::trav () {
    //for ( int i = 1; i < n; i++ )
    //cout << p[i].tcol << " " << p[i].id1 << " || ";
}