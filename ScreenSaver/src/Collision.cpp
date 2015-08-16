#include "Structures.h"
#include "Variables.h"
#include "Update.h"
#include <math.h>


#ifdef DEBUG_BALL               //Prints the Veloctiies of the Ball along with the ball ID
#define DEBUG( w, x, y, z ) do { cout << w << "x_vel = " << x << " | y_vel = " << y << " | z_vel = " << z << endl; } while (0)
#else
#define DEBUG( w, x, y, z )
#endif

#ifdef DEBUG_WALL               //Prints  the wall number and the ball ID
#define DEBUG2(w, x, y) do { cout << "Display.cpp -> collision_wall() :" <<endl; cout << "ball_id = " << w << " | wall_id =  " << x << " " << y << endl << endl; } while (0)
#else
#define DEBUG2(w, x, y)
#endif

void collision_ball ( long id, int from, b &ball1, b &ball2) {
    float dx = ball1.x - ball2.x;
    float dy = ball1.y - ball2.y;
    float dz = ball1.z - ball2.z;
    
    float sum_radius = pow(ball1.rad + ball2.rad,2);
    
    float dxi = ball1.xi - ball2.xi;
    float dyi = ball1.yi - ball2.yi;
    float dzi = ball1.zi - ball2.zi;
    
#ifdef DEBUG_BALL
    cout << "Display.cpp -> collision_ball() :" << endl;
#endif
    float c1 = ((2*pow(ball2.rad,3))/((pow(ball2.rad,3) + pow(ball1.rad,3))*sum_radius)) ;
    float c2 = ((2*pow(ball1.rad,3))/((pow(ball2.rad,3) + pow(ball1.rad,3))*sum_radius)) ;
    DEBUG ( "BALL1 before collision -> ", ball1.xi, ball1.yi, ball1.zi );
    ball1.xi = ball1.xi - c1*dx*(dx*dxi + dy*dyi + dz*dzi);     //Calculate New Velcoity of Own Ball
    ball1.yi = ball1.yi - c1*dy*(dx*dxi + dy*dyi + dz*dzi);
    ball1.zi = ball1.zi - c1*dz*(dx*dxi + dy*dyi + dz*dzi);
    DEBUG ( "BALL1 after collision -> ", ball1.xi, ball1.yi, ball1.zi );
    
    message temp;
    temp.data = ball2;
    temp.from = id;
    temp.state = 2;
    
    DEBUG ( "BALL2 before collision -> ", temp.data.xi, temp.data.yi, temp.data.zi );
    temp.data.xi = ball2.xi + c2*dx*(dx*dxi + dy*dyi + dz*dzi);     //Calculates the new velocity of the other ball
    temp.data.yi = ball2.yi + c2*dy*(dx*dxi + dy*dyi + dz*dzi);
    temp.data.zi = ball2.zi + c2*dz*(dx*dxi + dy*dyi + dz*dzi);
    DEBUG ( "BALL2 after collision -> ", temp.data.xi, temp.data.yi, temp.data.zi );
#ifdef DEBUG_BALL
    cout << endl;
#endif
    mail[from].push ( temp );               //Sends the data of collision to the other ball
}

void collision_wall ( int id, b &ball, char wall, int dir ) {
    bool wallcol = false;
    switch ( wall ) {                       //In case of Wall Collsion, Updates Velocity
        case 'x' : if ( ball.rad + dir*ball.x >= CUBESIDE/2 ) {
            ball.xi = -1*dir*fabs ( ball.xi );
            wallcol = true;
        }
            break;
        case 'y' : if ( ball.rad + dir*ball.y >= CUBESIDE/2 ) {
            ball.yi = -1*dir*fabs ( ball.yi );
            wallcol = true;
        }
            break;
        case 'z' : if ( ball.rad + dir*ball.z >= CUBESIDE/2 ) {
            ball.zi = -1*dir*fabs ( ball.zi );
            wallcol = true;
        }
            break;
    }
    if ( wallcol ) {
        DEBUG2 ( id, wall, dir );
        col[id]=iter;
        for(int i = 0 ; i < n ; i++ ) {
            if(id == i)
                continue;
            ifcollide[id]=BLINKLENGTH;                  //INitalizing the blinklength For the GUI effect of the red balls after collison
            update_pqueue( id, i, ball, fordisp[i]);
        }
    }
}