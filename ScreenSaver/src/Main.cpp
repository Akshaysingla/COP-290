#include <GL/glut.h>
#include <GL/glui.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <queue>
#include <vector>
#include "Structures.h"
#include "PriorityQueueArray.h"
#include "Variables.h"
#include "Display.h"
#include "Menu.h"
#include "Update.h"

using namespace std;


/* Initialize Variables */
float view_rotate_init[16] = {0.942592, -0.0607691 ,0.328369 ,0 ,-0.0703547 ,0.925096 ,0.373157, 0, -0.326449, -0.374838, 0.867714 ,0 ,0 ,0 ,0 ,1 };
float view_rotate[16];

int moving = 1;
bool begin1 = false;
GLfloat  asp = 1.77;
pthread_mutex_t mtx;
pthread_mutex_t temp;
pthread_barrier_t bar1;
pthread_barrier_t bar2;
int iter = 0;
int *istamp;
int *ifcollide;
PriorityQueueArray collision;

int window_id;

GLUI *glui_st;
GLUI_Rotation *sph_rot;
GLUI_Listbox *background;
GLUI_Listbox *boxtexture;

int background_num = 0;
int boxtexture_num = 4;

int num_of_textures = 8;

int dimension = 1;			// 0 for 2D, 1 for 3D
int selball = 0;

int n=5;

vector< queue<message> > mail;       //number of balls
int *col;
b *fordisp;
int c;
char** v;
float alpha = 2.0;

// Material properties
float Ka = 0.2;
float Kd = 0.4;
float Ks = 0.4;
float Kp = 0.5;

int global_w;
int global_h;

bool screen_mode = false;

void *iterate ( void *tid ) {
    pqitem first;       //Top of PQueue
    while(1) {
    	while ( !moving ) {}    //Pause
        pthread_barrier_wait (&bar1);
        first = collision.top();
        while ( first.tcol < iter && !collision.empty()  ) {    //Removing invalid collisons
            collision.pop();
            first = collision.top();
        }
        while( first.tcol == iter ) {       //Valid Collisions
            if (collision.empty()){
                break;
            }
            if(col[first.id1] > first.tid || col[first.id2] > first.tid) {  //Removing further invalid collisions
                collision.pop();
                first = collision.top();
                continue;
            }
            else {
                collision.pop();
                col[first.id1]=iter + 1;        //Setting previous collision of balls
                col[first.id2]=iter + 1;
                message m;                      //Sending collision message to ball
                m.from = first.id2;
                m.data = first.ball2;
                m.data.x = m.data.x + m.data.xi*(iter - first.tid);
                m.data.y = m.data.y + m.data.yi*(iter - first.tid);
                m.data.z = m.data.z + m.data.zi*(iter - first.tid);
                m.state = 1;
                mail[first.id1].push ( m );
                first = collision.top();
            }
        }
        #ifdef ITERATION
        if ( iter%500 == 0)
            cout << "Iteration Number : " << iter << endl;
        #endif
        #ifdef PQUEUE_TOP
         if ( collision.top().tcol != -1 )   
            cout << "Top Element in Priority Queue : " << collision.top().tcol << endl;
        #endif
        iter++;
    }
}

int main(int argc, char** argv) {
    if (argc ==2){
        n = atoi(argv[1]);
        if ( n > 25 ) {
            cout << "Error : Number of Balls exceeds limit ( Limit : 25 )" << endl;
            exit(0);
        }
    }
    srand ( time(NULL));
    for ( int i = 0; i < 16; i++ )
    	view_rotate[i] = view_rotate_init[i];
    pthread_t disp;
    pthread_create ( &disp, NULL, maindisplay, (void*)1 );      //Creating Display Thread
    while ( !begin1 ) {}
    pthread_t *t, it1;
    pthread_barrier_init(&bar1, 0, n+1);
    pthread_barrier_init(&bar2, 0, n);
    pthread_mutex_init ( &mtx, 0 );
    col = new int[n];
    fordisp = new b[n];
    istamp = new int[n];
    ifcollide = new int[n];
    c = argc;
    v = argv;
    for ( int i = 0; i < n; i++ ) {
        col[i] = -1;
        queue<message> q;
        mail.push_back ( q );
    }
    t = new pthread_t[n];
    for ( int i = 0; i < n; i++ ) {     //Creating individual ball threads
    	ifcollide[i]=false;
        pthread_create ( &t[i], NULL, update, (void*)i );
    }
    pthread_create ( &it1, NULL, iterate, (void*)0 );   //Creating thread for managing iterations
    void **status;
    pthread_join ( disp, status );
    pthread_mutex_destroy ( &mtx );
    pthread_barrier_destroy(&bar2);
    pthread_barrier_destroy(&bar1);
}