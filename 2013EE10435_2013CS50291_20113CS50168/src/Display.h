#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <GL/glut.h>

void init_material(float Ka, float Kd, float Ks, float Kp, float Mr, float Mg, float Mb);
void draw_background();
void reshape(GLint w, GLint h);
void init_light(int light_source, float Lx, float Ly, float Lz, float Lr, float Lg, float Lb);
void display();
void init();
void timer(int v);
void start ( int i );
void myGlutIdle( void );
void keyboard(unsigned char, int ,int);
void *maindisplay ( void *id );

#endif
