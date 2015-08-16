#include <GL/glut.h>
#include "Structures.h"
#include "Variables.h"
#include "Collision.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>


float random_range(int x , int y) {
    return x + ((rand()%((y-x)*100)))/100.0 ;
}

void Initialize(long id, b &ball) { //To Initialize the Ball Information (Position, Velocity, Colour)
    /*  //To Randomize The Initial Velocities
     
     ball.x =  random_range(-CUBESIDE/2 + 2*ball.rad,CUBESIDE/2 - 2*ball.rad);
     ball.y = random_range(-CUBESIDE/2 + 2*ball.rad,CUBESIDE/2 - 2*ball.rad);
     ball.z = random_range(-CUBESIDE/2 + 2*ball.rad,CUBESIDE/2 - 2*ball.rad);
     */
    //All Balls Start off at the centre
     ball.rad = random_range(RADIUS*0.6,RADIUS);
    ball.x = 0;
    ball.y = 0;
    ball.z = 0;
    //Setting Initial Velocities
    ball.xi = random_range(-VELOCITY,VELOCITY);
    ball.yi = random_range(-VELOCITY,VELOCITY);
    ball.zi = random_range(-VELOCITY,VELOCITY);
    //Setting Ball Colours
    ball.r = random_range(0,1);
    ball.g = random_range(0,1);
    ball.b = random_range(0,1);
    fordisp[id] = ball; //Updating global array for displaying files
    col[id]=-1; //Setting Previous Collision Invalid
}

void update_pqueue (long id, int i, b ball1, b ball2 ) {    //Uppating the Priority Queue wrt the Collision Times
    GLfloat dx, dy, dz, dvx, dvy,dvz, dist , min_dist , alpha , beta;
    int tempcol ;
    pqitem ins; //Item of Priority Queue
    ins.id1=id; //Id of Ball 1
    ins.id2=i;  //Id of Ball 2
    ins.tid=iter;   //Time (Iteration number) of creation of Item.
    ins.ball1=ball1;    //Data of Ball 1
    ins.ball2=ball2;    //Data of Ball 2
    
    //Calculations for Calculating Next collision
    dx = ball1.x - ball2.x;
    dy = ball1.y - ball2.y;
    dz = ball1.z - ball2.z;
    
    dvx = ball1.xi - ball2.xi;
    dvy = ball1.yi - ball2.yi;
    dvz = ball1.zi - ball2.zi;
    
    min_dist = pow((fordisp[id].rad + fordisp[i].rad),2);
    dist = dx*dx + dy*dy + dz*dz ;
    alpha = dx*dvx + dy*dvy + dz*dvz;
    beta = dvx*dvx + dvy*dvy + dvz*dvz;
    
    tempcol = (int)(((-1)*alpha - sqrt(alpha*alpha - (dist - min_dist)*(beta)))/beta);
    
    if(tempcol < 0 || tempcol > INFINITE - 2){
        ins.tcol = -1;  //Setting the Collision time invalid
    }
    else{
        ins.tcol = iter + tempcol;  //Setting Collision time
    }
    pthread_mutex_lock ( &mtx );
    collision.push(ins);    //Pushing the item into PQueue
    pthread_mutex_unlock ( &mtx );
}

void *update ( void *tid ) {    //Threads for individual balls
    b ball;
    long id = (long)tid;
    Initialize(id, ball);
    pthread_barrier_wait (&bar2);
    for(int i = 0 ; i < n ; i++ ) {     //Sending Collision time initially
        if(id==i)
            continue;
        update_pqueue (id, i, fordisp[id], fordisp[i] );
    }
    int tempcol;
    fordisp[id]=ball;
    
    while (1) {
        ball.x = ball.x + ball.xi;      //Updating Velocities
        ball.y = ball.y + ball.yi;
        ball.z = ball.z + ball.zi;
        
        message m;
        fordisp[id] = ball;
        
        while ( !mail[id].empty () ) {      //Checking Mail
            m = mail[id].front();
            mail[id].pop ();
            if( m.state == 1) {             //Collision Message
                if( m.from >= 0 ) {
                    collision_ball(id, m.from, ball, m.data);
                    ifcollide[id]=BLINKLENGTH;
                }
                if ( dimension == 0 ) {
                    ball.z = 0;
                    ball.zi = 0;
                }
                fordisp[id] = ball;
            }
            else if ( m.state == 2 ) {      //Message containing updated Data after collision
                ball = m.data;
                if ( dimension == 0 ) {
                    ball.z = 0;
                    ball.zi = 0;
                }
                ifcollide[id]=BLINKLENGTH;
                pthread_mutex_lock ( &mtx );
                fordisp[id] = ball;
                pthread_mutex_unlock ( &mtx );
            }
            else if ( m.state == 3 ) {      //Increase Velocity
                ball.xi *= 1.1;
                ball.yi *= 1.1;
                ball.zi *= 1.1;
            }
            else if ( m.state == 4 ) {      //Decrease Velocity
                ball.xi *= 0.9;
                ball.yi *= 0.9;
                ball.zi *= 0.9;
            }
            else if ( m.state == 5 ) {     //Setting Speed = 0
                ball.xi = 0;
                ball.yi = 0;
                ball.zi = 0;
            }
            else if ( m.state == 6 ) {     //2D Mode
                ball.z = 0;
                ball.zi = 0;
            }
            else if ( m.state == 7 ) {     //Changing from 2D to 3D
                ball.zi = random_range(-VELOCITY,VELOCITY);
            }
            
            fordisp[id]=ball;
            for(int i = 0 ; i < n ; i++ ) { //Updating Priority Queue
                if(id == i || i == m.from)
                    continue;
                update_pqueue( id, i, ball, fordisp[i]);
            }
            
            
        }
        collision_wall ( id, ball, 'x', 1 );    //Checking for Wall collisions
        collision_wall ( id, ball, 'x', -1 );
        collision_wall ( id, ball, 'y', 1 );
        collision_wall ( id, ball, 'y', -1 );
        collision_wall ( id, ball, 'z', 1 );
        collision_wall ( id, ball, 'z', -1 );
        pthread_barrier_wait (&bar1);
        usleep ( 2000 );
    }
}