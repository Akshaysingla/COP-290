#ifndef VARIABLE_H_
#define VARIABLE_H_

#include <GL/glut.h>
#include <vector>
#include <queue>
#include <pthread.h>
#include <GL/glui.h>
#include "Structures.h"
#include "PriorityQueueArray.h"

using namespace std;

#define INFINITE 2147483647

#define VELOCITY 1      //Ball Velocity Range
#define BLINKLENGTH 10      //Time taken from Red After collision to return no Normal Colour
#define RADIUS 40       //Radius of Ball
#define CUBESIDE 850    //Cubeside/1

extern float view_rotate_init[16];      //Default Rotation View for 3D Box
extern float view_rotate[16];

extern int moving;          //Pause/Unpause
extern bool begin1;
extern GLfloat  asp;
extern pthread_mutex_t mtx;
extern pthread_mutex_t temp;
extern pthread_barrier_t bar1;
extern pthread_barrier_t bar2;
extern int iter;
extern int *istamp;
extern int *ifcollide;
extern PriorityQueueArray collision;

extern int window_id;

extern GLUI *glui_st;           //GLUI Variables
extern GLUI_Rotation *sph_rot;
extern GLUI_Listbox *background;
extern GLUI_Listbox *boxtexture;
extern int background_num;
extern int boxtexture_num;

extern int dimension;			// 0 for 2D, 1 for 3D
extern int selball;             //Selected Ball Number

extern vector < queue<message> > mail;      //Global Mail-Box
extern int n;       //number of balls
extern int *col;    //
extern b *fordisp;  //Array Storing balls for all displays
extern int c;
extern char** v;
extern float alpha;

// Material properties-Ambience,Diffusion,Specular,Shininess
extern float Ka;
extern float Kd;
extern float Ks;
extern float Kp;

extern int global_w;        //Width
extern int global_h;        //Height

extern bool screen_mode;

#endif
