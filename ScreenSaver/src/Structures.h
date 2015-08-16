#ifndef STRUCTURES_H_
#define STRUCTURES_H_

#include <GL/glut.h>

struct b {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat xi;
    GLfloat yi;
    GLfloat zi;
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat rad;
};

struct pqitem {
    int id1, id2, tcol, tid;
    b ball1, ball2;
};

struct message {
    int from;
    int state;
    b data;
};


#endif