#include <GL/glut.h>
#include <iostream>
#include "Structures.h"
#include <stdlib.h>
#include <GL/glui.h>
#include <unistd.h>
#include "Maze.h"
#include <math.h>
#include "SDL/SDL.h"
#include <vector>
#include <SDL/SDL_image.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Variables.h"
#include "Net.h"

using namespace std;
using namespace net;

bool mouseWarp;
const int ProtocolId = 0x11223344;
const float DeltaTime = 1.0f / 120.0f;
const float SendRate = 1.0f / 120.0f;
const float TimeOut = 10.0f;
const int PacketSize = 512;
const int global_h =1080;
const int global_w = 1920;
int DispScreen;
int SpellSelected;
#define HEALTHMAX 10
#define MANAMAX 100
#define SHIELD 10
#define PLAYERMAX 6

const float asp = 16/9;
int main_window;
int display_which ;
ReliableConnection connection( ProtocolId, TimeOut );
int time_left = 700;
int ToalBullets = 0;
int c;
int players;
int AI_number;
char** v;
int HealthAngle;
const int N = 22;
float powerUpCubeLength = 0.25;
float playerRadius = 0.25;
//const int NumPlayers;
int SpellID = 1;
GLfloat SpellColour[3][3] = {{0.855f, 0.647f, 0.125f}, {0.753f, 0.753f, 0.753f}, {0.886f, 0.345f, 0.133f}};
GLfloat PlayerColour[7][3] = {{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, {1.0, 1.0, 1.0}};
int playerFreeze[PLAYERMAX];

Mode mode;
GLUI *sub_w;


bool AI_Thread[PLAYERMAX];

bool display_player[PLAYERMAX];

GameType gamestyle;

//const int numpowerUp = N*N/16;
const int numpowerUp = 25;
powerUp powerUps[numpowerUp];
int powerUpangle = 0;

bool keyup;
float view_rotate[16] = {0.942592, -0.0607691 ,0.328369 ,0 ,-0.0703547 ,0.925096 ,0.373157, 0, -0.326449, -0.374838, 0.867714 ,0 ,0 ,0 ,0 ,1 };
float view_rotate_init[16];

float edgeLength = 2.0;
float wallThickness = 0.2;
float mazeHeight = 1.2;
grid** maze;
float _angle = 0.0f;
GLuint texture[8];
int torangle = 0;

int mynumber;

int networkplayers;

float bulletRadius = 0.025;

float lx=1;
float ly=0;
float prevlx, prevly;
float prevx, prevy;
int prevgridx, prevgridy;
float angle = 0.0f;
float deltaAngle;
float xOrigin = 500;


//int playerStun[players];
//bool ***visit = new bool**[PLAYERMAX];
bool visit[PLAYERMAX][N][N];

int window_id;

GLUI *glui_st;           //GLUI Variables

pos otherPlayers[PLAYERMAX];

//pos otherPlayers[mynumber];

pos rec_pos[PLAYERMAX];
float lx2;
float ly2;


vector<spell> RecSpellCollect;
spell tempspellRec;

vector<spell*> spell_on_screen;

vector<spell> PendingBulletsSend;


playerstats mystat[PLAYERMAX];


int orbs[4] = { 6,6,6,6 };

int count_down = 0;

GLUquadricObj *obj1 = gluNewQuadric();
GLUquadricObj *obj2 = gluNewQuadric();
GLUquadricObj *obj3 = gluNewQuadric();


void initializePowerups () {
    int trand=0;
    int powerNo = 0;
    
    for(int i = 0 ; i < N/4 ; i++ ) {
        for(int j = 0; j < N/4 ; j++ ) {
            powerUps[powerNo].exist = false;
            powerUps[powerNo].type = 6;
            powerUps[powerNo].timestamp = rand()%20;
            powerUps[powerNo].gridx = rand()%4 + 4*i;
            powerUps[powerNo++].gridy = rand()%4 + 4*j;
            
        }
    }

    int temp = 0;
    while(temp < numpowerUp ) {
        //trand = (trand+7)%50;
        trand = rand()%25;
        if(powerUps[trand].type==6) {
            if(temp < 10)
                powerUps[trand].type=0;
            else if (temp < 20)
                powerUps[trand].type=1;
            else
                powerUps[trand].type=2;
            temp++;
        }
    }
}

void updatepowerUps (float increment, int spawntime) {
    for( int i = 0; i < numpowerUp ; i++ ) {
        if(!powerUps[i].exist) {
            powerUps[i].timestamp+=increment;
            if( powerUps[i].timestamp > spawntime ) {
                powerUps[i].exist = true;
            }
        }

    }
    for( int i = 0 ; i < players ; i++ ) {
        if (mystat[i].shield > 0){
        	mystat[i].shield -= increment;        
        	if (mystat[i].shield < 0){
            	mystat[i].shield = 0;
        	}
    	}
	}
}


void checkpowerUp() {
	for (int j = 0 ; j < players ; j++ ){
    	for(int i = 0 ; i < numpowerUp ; i++ ){
        if(otherPlayers[j].gridx == powerUps[i].gridx && otherPlayers[j].gridy == powerUps[i].gridy ) {
            if (otherPlayers[j].x*otherPlayers[j].x + otherPlayers[j].y*otherPlayers[j].y < (playerRadius + powerUpCubeLength/2)*(playerRadius + powerUpCubeLength/2)) {
                powerUps[i].timestamp = 0;
                if (powerUps[i].exist){
                if (powerUps[i].type == 0){
                    if (mystat[j].health != HEALTHMAX){
                    mystat[j].health += 1;
                    }
                }
                else if (powerUps[i].type == 1){
                    if (mystat[j].mana != MANAMAX){
                        mystat[j].mana += 30;
                    }
                }
                else{
                    mystat[j].shield += SHIELD;
                    }
                powerUps[i].exist = false;
                break;
                }
            }
        }
    	}
	}	
}


void maze_to_packet ( grid* data[], unsigned char toSend[] ) {
    for ( int i = 0; i < N; i++ ) {
        for ( int j = 0; j < N; j++ ) {
            int cur = (N*i + j)/2;
            for ( int k = 0; k < 4; k++ ) {
                toSend[cur] = toSend[cur] << 1;
                if ( data[i][j].edge[k] )
                    toSend[cur]++;
            }
        }
    }
}

void packet_to_maze ( unsigned char data[], grid* toRec[] ) {
    for ( int i = 1; i <= (N*N)/2; i++ ) {
        for ( int j = 2*i-1; j >= 2*i-2; j-- ) {
            for ( int k = 3; k >= 0; k-- ) {
                toRec[j/N][j%N].edge[k] = data[i-1] % 2;
                data[i-1] = data[i-1] >> 1;
            }
        }
    }
}

void check_coords () {
    if ( maze[otherPlayers[mynumber].gridx][otherPlayers[mynumber].gridy].edge[0] && otherPlayers[mynumber].y <= -(edgeLength-wallThickness-2*playerRadius)/2 )
        otherPlayers[mynumber].y = -(edgeLength-wallThickness-2*playerRadius)/2;
    if ( maze[otherPlayers[mynumber].gridx][otherPlayers[mynumber].gridy].edge[2] && otherPlayers[mynumber].y >= +(edgeLength-wallThickness-2*playerRadius)/2 )
        otherPlayers[mynumber].y = +(edgeLength-wallThickness-2*playerRadius)/2;
    if ( maze[otherPlayers[mynumber].gridx][otherPlayers[mynumber].gridy].edge[3] && otherPlayers[mynumber].x <= -(edgeLength-wallThickness-2*playerRadius)/2 )
        otherPlayers[mynumber].x = -(edgeLength-wallThickness-2*playerRadius)/2;
    if ( maze[otherPlayers[mynumber].gridx][otherPlayers[mynumber].gridy].edge[1] && otherPlayers[mynumber].x >= +(edgeLength-wallThickness-2*playerRadius)/2 )
        otherPlayers[mynumber].x = +(edgeLength-wallThickness-2*playerRadius)/2;
    //cout << otherPlayers[mynumber].gridx << " " << otherPlayers[mynumber].gridy << endl;
    if ( otherPlayers[mynumber].y > +edgeLength/2 ) {
        otherPlayers[mynumber].gridy += 1;
        otherPlayers[mynumber].y += -edgeLength;
    }
    if ( otherPlayers[mynumber].y < -edgeLength/2 ) {
        otherPlayers[mynumber].gridy += -1;
        otherPlayers[mynumber].y += +edgeLength;
    }
    if ( otherPlayers[mynumber].x > +edgeLength/2 ) {
        otherPlayers[mynumber].gridx += 1;
        otherPlayers[mynumber].x += -edgeLength;
    }
    if ( otherPlayers[mynumber].x < -edgeLength/2 ) {
        otherPlayers[mynumber].gridx += -1;
        otherPlayers[mynumber].x += +edgeLength;
    }
    checkpowerUp();
}


void evictrandomly(int i){
    if (rand()%2 == 0){
        otherPlayers[i].gridy = rand()%N;
        if (rand()%2 == 0){
            otherPlayers[i].gridx = N-1;
        }
        else{
            otherPlayers[i].gridx = 0;
        }
    }
    else{
        otherPlayers[i].gridx = rand()%N;
        if (rand()%2 == 0){
            otherPlayers[i].gridy = N-1;
        }
        else{
            otherPlayers[i].gridy = 0;
        }

    }
}


int check_coords_bullet ( spell* checkSpell ) {
    if ( maze[checkSpell->gridx][checkSpell->gridy].edge[0] && checkSpell->y <= -(edgeLength-wallThickness-2*bulletRadius)/2 )
        return 0;
    if ( maze[checkSpell->gridx][checkSpell->gridy].edge[2] && checkSpell->y >= +(edgeLength-wallThickness-2*bulletRadius)/2 )
        return 0;
    if ( maze[checkSpell->gridx][checkSpell->gridy].edge[3] && checkSpell->x <= -(edgeLength-wallThickness-2*bulletRadius)/2 )
        return 0;
    if ( maze[checkSpell->gridx][checkSpell->gridy].edge[1] && checkSpell->x >= +(edgeLength-wallThickness-2*bulletRadius)/2 )
        return 0;
    float cur_x = edgeLength/2 + (checkSpell->gridx)*edgeLength + checkSpell->x;
    float cur_y = edgeLength/2 + (checkSpell->gridy)*edgeLength + checkSpell->y;
    if ( pow ( N/2*edgeLength - cur_x, 2 ) + pow ( N/2*edgeLength - 0.6 - cur_y, 2 ) < pow ( 0.15, 2 ) ) {
        if ( mode == Host )
            orbs[0] = checkSpell->my_shooter;
        return 0;
    }
    if ( pow ( N/2*edgeLength - cur_x, 2 ) + pow ( N/2*edgeLength + 0.6 - cur_y, 2 ) < pow ( 0.15, 2 ) ) {
        if ( mode == Host )
            orbs[2] = checkSpell->my_shooter;
        return 0;
    }
    if ( pow ( N/2*edgeLength - 0.6 - cur_x, 2 ) + pow ( N/2*edgeLength - cur_y, 2 ) < pow ( 0.15, 2 ) ) {
        if ( mode == Host )
            orbs[3] = checkSpell->my_shooter;
        return 0;
    }
    if ( pow ( N/2*edgeLength + 0.6 - cur_x, 2 ) + pow ( N/2*edgeLength - cur_y, 2 ) < pow ( 0.15, 2 ) ) {
        if ( mode == Host )
            orbs[1] = checkSpell->my_shooter;
        return 0;
    }

    for(int i = 0; i < players ; i++ ) {
        if( i == checkSpell-> my_shooter )
            continue;
        if(checkSpell->gridx == otherPlayers[i].gridx && checkSpell->gridy == otherPlayers[i].gridy ) {
            if((otherPlayers[i].x - checkSpell->x)*(otherPlayers[i].x - checkSpell->x) + (otherPlayers[i].y - checkSpell->y)*(otherPlayers[i].y - checkSpell->y) < playerRadius*playerRadius){
                if( checkSpell->type == 0 ) {
                    if(mystat[i].shield == 0) {
                        if( !otherPlayers[i].ai ) {
                            evictrandomly(i);
                        }
                        else {
                            if(mode==Host) {
                                display_player[i] = false;
                                AI_Thread[i] = false;
                            }
                        }
                    }
                    return 0;
                }

                else if (checkSpell->type == 1) {
                    if (mystat[i].shield == 0){
                        mystat[i].hit = 15;
                        mystat[i].health --;
                        //st1->set_text ( "yoo" );
                        if (mystat[i].health == 0){
                            if (i == mynumber){
                                evictrandomly(i);
                            }
                        else if ( mode == Host && otherPlayers[i].ai){
                            // Delete AI using thread no.
                            display_player[i] = false;
                            AI_Thread[i] = false;
                           // cout << "Sent Message" << endl;
                       
                        }
                        else{
                            otherPlayers[i].gridx = 0 ;
                            otherPlayers[i].gridy = 0 ;
                            }
                            mystat[i].health = HEALTHMAX;
                            mystat[i].hit = 0 ;
                            mystat[i].shield = 0;
                            mystat[i].mana = MANAMAX/2 ;
                        }
                    }
                    return 0;
                }
                else {
                    if( mystat[i].shield == 0 )
                        playerFreeze[i] = 100;
                }
                return 0;
            }
        }
    }
    //cout << checkSpell->gridx << " " << checkSpell->gridy << endl;
    if ( checkSpell->y > +edgeLength/2 ) {
        checkSpell->gridy += 1;
        checkSpell->y += -edgeLength;
    }
    if ( checkSpell->y < -edgeLength/2 ) {
        checkSpell->gridy += -1;
        checkSpell->y += +edgeLength;
    }
    if ( checkSpell->x > +edgeLength/2 ) {
        checkSpell->gridx += 1;
        checkSpell->x += -edgeLength;
    }
    if ( checkSpell->x < -edgeLength/2 ) {
        checkSpell->gridx += -1;
        checkSpell->x += +edgeLength;
    }
    return 1;
}

void LoadGLTextures( ) {
    /* Create storage space for the texture */
    SDL_Surface *TextureImage[2];
    const char* names[8] = {"resources/back2.jpg", "resources/back2.jpg", "resources/health.jpg", "resources/spell.jpg", "resources/shield.jpg", "resources/win.jpg", "resources/lose.jpg", "resources/error.jpg"};
    for (int i = 0 ; i< 8 ; i++){
        if ((TextureImage[0] = IMG_Load(names[i]))) {
            glGenTextures( 1, &texture[i] );
            glBindTexture( GL_TEXTURE_2D, texture[i] );
            glTexImage2D( GL_TEXTURE_2D, 0, 3, TextureImage[0]->w,
                         TextureImage[0]->h, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, TextureImage[0]->pixels );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        }
        //cout<<texture[0]<<endl;
        /* Free up some memory */
        if ( TextureImage[0] )
            SDL_FreeSurface( TextureImage[0] );
    }
}

void reshape(GLint w, GLint h) {
    glViewport(0, 0, w, h);
    double aspect = (double)w/ (double)h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 0.01, 1000.0);
    //glLoadIdentity();
    //glOrtho ( 0, 200, 0, 200, -100, 100 );
    //gluLookAt()
    gluLookAt ( edgeLength/2 + otherPlayers[mynumber].gridx*edgeLength + otherPlayers[mynumber].x - 2*edgeLength, edgeLength/2 + otherPlayers[mynumber].gridy*edgeLength + otherPlayers[mynumber].y, 2.0,
               edgeLength/2 + otherPlayers[mynumber].gridx*edgeLength + otherPlayers[mynumber].x, edgeLength/2 + otherPlayers[mynumber].gridy*edgeLength + otherPlayers[mynumber].y, 1.75,
               0, 0, 1 );
    //gluLookAt ( 10, 10, 50, 10, 10, 0, 0, 1, 0 );
    
}

void initRendering() {
	initializePowerups();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING); //Enable lighting
    
    //glEnable(GL_LIGHT4); //Enable light #0
    //glEnable(GL_LIGHT5); //Enable light #1
    //glEnable(GL_LIGHT6); //Enable light #0
    //glEnable(GL_LIGHT7); //Enable light #1
    glEnable(GL_NORMALIZE); //Automatically normalize normals
    glShadeModel(GL_SMOOTH); //Enable smooth shading
    LoadGLTextures();
    glBindTexture( GL_TEXTURE_2D, texture[0] );
    //Add ambient light
    GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 0.5f}; //Color (0.2, 0.2, 0.2)
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    
    //Add positioned light
    GLfloat lightColor0[] = {1.0f, 1.0f, 1.0f, 0.5f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos0[] = {0.0f, 0.0f, 2*mazeHeight, 0.2f}; //Positioned at (4, 0, 8)
    GLfloat lightColor0spec[] = {0.0f, 0.0f, 0.0f, 0.5f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    
    //Add directed light
    GLfloat lightColor1[] = {1.0f, 1.0f, 1.0f, 0.5f}; //Color (0.5, 0.2, 0.2)
    //Coming from the direction (-1, 0.5, 0.5)
    GLfloat lightPos1[] = {(N-1)*edgeLength, 0.0f, 2*mazeHeight, 0.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    
    GLfloat lightColor2[] = {1.0f, 1.0f, 1.0f, 0.5f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos2[] = {(N-1)*edgeLength,(N-1)*edgeLength,2*mazeHeight, 1.0f}; //Positioned at (4, 0, 8)
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT2, GL_POSITION, lightPos0);
    
    //Add directed light
    GLfloat lightColor3[] = {1.0f, 1.0f, 1.0f, 0.5f}; //Color (0.5, 0.2, 0.2)
    GLfloat lightColor3amb[] = {0.0f, 0.0f, 0.0f, 0.0f}; //Color (0.5, 0.2, 0.2)
    GLfloat lightPos3[] = {0.0f, (N-1)*edgeLength, 2*mazeHeight, 0.0f};
    GLfloat lightSpot[] = {0.0f, 0.0f, -1.0f};
    //glLightfv(GL_LIGHT3, GL_AMBIENT, lightColor1);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT3, GL_POSITION, lightPos1);
    //glLightfv(GL_LIGHT3,GL_SPOT_DIRECTION, lightSpot);
    //glLightf(GL_LIGHT3,GL_SPOT_EXPONENT, 1.0f);
    //glLightf(GL_LIGHT3,GL_LINEAR_ATTENUATION, 1.0f);
    
    
    GLfloat lightColor4[] = {1.0f, 1.0f, 1.0f, 0.5f }; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos4[] = {(N-1)*edgeLength/2, (N-1)*edgeLength/2, 10*mazeHeight, 1.0f}; //Positioned at (4, 0, 8)
    glLightfv(GL_LIGHT4, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT4, GL_POSITION, lightPos0);
    // glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.4f);
    
    glEnable(GL_LIGHT0); //Enable light #0
    glEnable(GL_LIGHT1); //Enable light #1
    glEnable(GL_LIGHT2); //Enable light #0
    glEnable(GL_LIGHT3); //Enable light #1
    glEnable(GL_LIGHT4); //Enable light #1
    
    keyup = false;
}

void drawCube( float x1, float y1, float x2, float y2, float h ) {
    
    glBegin(GL_QUADS);
    /* Front Face */
    glTexCoord2f( 0.0, 1 );
    glVertex3f( x1, y1, h );
    glTexCoord2f( 1, 1 );
    glVertex3f(  x2, y1, h );
    glTexCoord2f( 1, 0.0 );
    glVertex3f(  x2,  y2, h );
    glTexCoord2f( 0.0, 0.0 );
    glVertex3f( x1,  y2, h );
    
    /* Back Face */
    glTexCoord2f( 0.0, 0.0 );
    glVertex3f( x1, y1, 0 );
    glTexCoord2f( 0.0, 1 );
    glVertex3f( x1,  y2, 0 );
    glTexCoord2f( 1, 1 );
    glVertex3f(  x2,  y2, 0 );
    glTexCoord2f( 1, 0.0 );
    glVertex3f(  x2, y1, 0 );
    
    /* Top Face */
    glTexCoord2f( 1, 1 );
    glVertex3f( x1, y2, 0 );
    glTexCoord2f( 1, 0.0 );
    glVertex3f( x1,  y2,  h );
    glTexCoord2f( 0.0, 0.0 );
    glVertex3f(  x2,  y2,  h );
    glTexCoord2f( 0.0, 1 );
    glVertex3f(  x2, y2, 0 );
    
    /* Bottom Face */
    glTexCoord2f( 0.0, 1 );
    glVertex3f( x1, y1, 0 );
    glTexCoord2f( 1, 1 );
    glVertex3f(  x2, y1, 0 );
    glTexCoord2f( 1, 0.0 );
    glVertex3f(  x2, y1,  h );
    glTexCoord2f( 0.0, 0.0 );
    glVertex3f( x1, y1, h );
    
    /* Right face */
    glTexCoord2f( 0.0, 0.0 );
    glVertex3f( x2, y1, 0 );
    glTexCoord2f( 0.0, 1 );
    glVertex3f( x2,  y2, 0 );
    glTexCoord2f( 1, 1 );
    glVertex3f( x2,  y2,  h );
    glTexCoord2f( 1, 0.0 );
    glVertex3f( x2, y1,  h );
    
    /* Left Face */
    glTexCoord2f( 1, 0.0 );
    glVertex3f( x1, y1, 0 );
    glTexCoord2f( 0.0, 0.0 );
    glVertex3f( x1, y1,  h );
    glTexCoord2f( 0.0, 1 );
    glVertex3f( x1,  y2,  h );
    glTexCoord2f( 1, 1 );
    glVertex3f( x1,  y2, 0 );
    glEnd( );
}


void *shoot ( void *id ) {
    spell *temp;

    temp = new spell;
    temp->type = (long)id;
    //cout<<"bulletid "<<temp->type<<endl;
    //temp->type = SpellSelected;
    temp->x = prevx;
    temp->y = prevy;
    temp->gridx = prevgridx;
    temp->gridy = prevgridy;
    temp->lx = prevlx;
    temp->ly = prevly;
    temp->exist = 1;
    temp->my_shooter = mynumber;
    if (gamestyle == Multi){
        spell temp2;
        temp2.type = temp->type;
        temp2.x = prevx;
        temp2.y = prevy;
        temp2.gridx = prevgridx;
        temp2.gridy = prevgridy;
        temp2.lx = prevlx;
        temp2.ly = prevly;
        temp2.exist = 1;
        temp2.my_shooter = mynumber;
        PendingBulletsSend.push_back (temp2);
        //cout<<"SENT"<<temp->x<<" "<<temp->y<<" "<<temp->gridx<<" "<<temp->gridy<<" "<<temp->lx<<" "<<temp->ly<<" "<<endl;
    }
    spell_on_screen.push_back ( temp );
    while ( 1 ) {
        temp->exist = check_coords_bullet ( temp );
        if ( temp->exist == 0 ) {
            break;
        }
        temp->x += temp->lx*0.1;
        //cout << temp->x << endl;
        temp->y += temp->ly*0.1;
        //cout<<"yoyo"<<endl;
        usleep (10000);
        //cout<<"yoyo1"<<endl;
    }
    //usleep (10000);
    //cout<<"exit"<<endl;
    //pthread_exit ( NULL );
}

void *shoot_ai ( void *id ) {
    spell *temp;

    temp = new spell;
    int choose_spell = rand()%100 + 1;
    temp->type = 3;
    if ( choose_spell <= 5 && mystat[(long)id].mana >= 20 ) {
        mystat[(long)id].mana -= 20;
        temp->type = 0;
    }
    else if ( choose_spell > 25 && mystat[(long)id].mana >= 1 ) {
        mystat[(long)id].mana -= 1;
        temp->type = 1;
    }
    else if ( mystat[(long)id].mana >= 5 ) {
        mystat[(long)id].mana -= 5;
        temp->type = 2;
    }
    if ( temp->type == 3 )
        pthread_exit(NULL);
    //cout << choose_spell << endl;
    //cout<<"bulletid "<<temp->type<<endl;
    //temp->type = SpellSelected;
    temp->x = prevx;
    temp->y = prevy;
    temp->gridx = prevgridx;
    temp->gridy = prevgridy;
    temp->lx = prevlx;
    temp->ly = prevly;
    temp->exist = 1;
    temp->my_shooter = (long)id;
    if (gamestyle == Multi){
        spell temp2;
        temp2.type = temp->type;
        temp2.x = prevx;
        temp2.y = prevy;
        temp2.gridx = prevgridx;
        temp2.gridy = prevgridy;
        temp2.lx = prevlx;
        temp2.ly = prevly;
        temp2.exist = 1;
        temp2.my_shooter = (long)id;
        PendingBulletsSend.push_back (temp2);
        //cout<<"SENT"<<temp->x<<" "<<temp->y<<" "<<temp->gridx<<" "<<temp->gridy<<" "<<temp->lx<<" "<<temp->ly<<" "<<endl;
    }
    spell_on_screen.push_back ( temp );

    while ( 1 ) {
        temp->exist = check_coords_bullet ( temp );
        if ( temp->exist == 0 ) {
            //cout<<temp->type<<" <-Spellno "<<temp->my_shooter<<endl;
            break;
        }
        temp->x += temp->lx*0.1;
        //cout << temp->x << endl;
        temp->y += temp->ly*0.1;
        //cout<<"yoyo"<<endl;
        usleep (10000);
        //cout<<"yoyo1"<<endl;
    }
    //usleep (10000);
    //cout<<"exit"<<endl;
    //pthread_exit ( NULL );
}

int line_of_sight ( pos play1, pos play2 ) {
    float netx1 = edgeLength/2 + edgeLength*play1.gridx + play1.x;
    float nety1 = edgeLength/2 + edgeLength*play1.gridy + play1.y;
    float netx2 = edgeLength/2 + edgeLength*play2.gridx + play2.x;
    float nety2 = edgeLength/2 + edgeLength*play2.gridy + play2.y;
    int num = 1000;
    float dx = (netx2-netx1)/num;
    float dy = (nety2-nety1)/num;
    float los_x = play1.x;
    float los_y = play1.y;
    int los_gridx = play1.gridx;
    int los_gridy = play1.gridy;
    for ( int i = 0; i < num; i++ ) {
        if ( maze[los_gridx][los_gridy].edge[0] && los_y <= -(edgeLength-wallThickness)/2 )
            return 0;
        if ( maze[los_gridx][los_gridy].edge[2] && los_y >= +(edgeLength-wallThickness)/2 )
            return 0;
        if ( maze[los_gridx][los_gridy].edge[3] && los_x <= -(edgeLength-wallThickness)/2 )
            return 0;
        if ( maze[los_gridx][los_gridy].edge[1] && los_x >= +(edgeLength-wallThickness)/2 )
            return 0;
        los_x += dx;
        los_y += dy;
        if ( los_y > +edgeLength/2 ) {
            los_gridy += 1;
            los_y += -edgeLength;
        }
        if ( los_y < -edgeLength/2 ) {
            los_gridy += -1;
            los_y += +edgeLength;
        }
        if ( los_x > +edgeLength/2 ) {
            los_gridx += 1;
            los_x += -edgeLength;
        }
        if ( los_x < -edgeLength/2 ) {
            los_gridx += -1;
            los_x += +edgeLength;
        }
    }
    return 1;
}

void circle_around ( int newID ) {
    int deltax, deltay;
    while ( AI_Thread[newID] ) {
        if ( otherPlayers[newID].gridx == N/2 - 2 && otherPlayers[newID].gridy != N/2-2 ) {
            deltax = 0;
            deltay = -1;
        }
        else if ( otherPlayers[newID].gridy == N/2 - 2 && otherPlayers[newID].gridx != N/2+1 ) {
            deltax = +1;
            deltay = 0;
        }
        else if ( otherPlayers[newID].gridx == N/2 +1 && otherPlayers[newID].gridy != N/2+1 ) {
            deltax = 0;
            deltay = +1;
        }
        else if ( otherPlayers[newID].gridy == N/2 +1 && otherPlayers[newID].gridx != N/2-2 ) {
            deltax = -1;
            deltay = 0;
        }
        if ( playerFreeze[newID] == 0 ) {
            vector<int> shoot_select;
            for ( int i = 0; i < players; i++ ) {
                if ( i == newID )
                    continue;
                if ( line_of_sight ( otherPlayers[newID], otherPlayers[i] ) ) {
                    shoot_select.push_back(i);
                }
            }
            for ( int i = 0; i < 4; i++ ) {
                if ( orbs[i] != newID ) {
                    shoot_select.push_back ( 10 + i );
                    shoot_select.push_back ( 10 + i );
                }
            }
            if ( shoot_select.size () != 0 ) {
                int select = shoot_select[rand()%shoot_select.size()];
                //cout << select << endl;
                float netx1 = edgeLength/2 + edgeLength*otherPlayers[newID].gridx + otherPlayers[newID].x;
                float nety1 = edgeLength/2 + edgeLength*otherPlayers[newID].gridy + otherPlayers[newID].y;
                float netx2, nety2;
                if ( select < 10 ) {
                    netx2 = edgeLength/2 + edgeLength*otherPlayers[select].gridx + otherPlayers[select].x;
                    nety2 = edgeLength/2 + edgeLength*otherPlayers[select].gridy + otherPlayers[select].y;
                }
                else {
                    switch ( select - 10 ) {
                        case 0: netx2 = (N/2)*edgeLength;
                                nety2 = (N/2)*edgeLength - 0.6;
                                break;
                        case 1: netx2 = (N/2)*edgeLength + 0.6;
                                nety2 = (N/2)*edgeLength;
                                break;
                        case 2: netx2 = (N/2)*edgeLength;
                                nety2 = (N/2)*edgeLength + 0.6;
                                break;
                        case 3: netx2 = (N/2)*edgeLength - 0.6;
                                nety2 = (N/2)*edgeLength;
                                break;
                    }
                }
                float sq = sqrt ( pow ( netx2 - netx1, 2) + pow ( nety2 - nety1, 2 ) );
                prevlx = (netx2-netx1)/sq;;
                prevly = (nety2-nety1)/sq;
                prevgridx = otherPlayers[newID].gridx;
                prevgridy = otherPlayers[newID].gridy;
                prevx = otherPlayers[newID].x;
                prevy = otherPlayers[newID].y;
                //cout << newID << " " << SpellID << " " << lx << " " << ly << endl;
                pthread_t t;
                pthread_create ( &t, NULL, shoot_ai, (void*)newID );
            }
            int move_num = edgeLength*100;
            for ( int i = 0; i < move_num; i++ ) {
                otherPlayers[newID].x += deltax*0.01;
                otherPlayers[newID].y += deltay*0.01;
                if ( otherPlayers[newID].x > +edgeLength/2 ) {
                    otherPlayers[newID].x += -edgeLength;
                    otherPlayers[newID].gridx++;
                }
                if ( otherPlayers[newID].x < -edgeLength/2 ) {
                    otherPlayers[newID].x += +edgeLength;
                    otherPlayers[newID].gridx--;
                }
                if ( otherPlayers[newID].y > +edgeLength/2 ) {
                    otherPlayers[newID].y += -edgeLength;
                    otherPlayers[newID].gridy++;
                }
                if ( otherPlayers[newID].y < -edgeLength/2 ) {
                    otherPlayers[newID].y += +edgeLength;
                    otherPlayers[newID].gridy--;
                }
                usleep ( 7500 );
            }
        }
    }
}

void* depthfirst ( void* id ) {

    int newID = (long)id;
    int pre_fetch = N*N;
    visit[newID][otherPlayers[newID].gridx][otherPlayers[newID].gridy] = 1;
    vector<node> path;
    path.push_back ( node ( otherPlayers[newID].gridx, otherPlayers[newID].gridy ) );
    //cout<<"dfs"<<endl;
    if ( AI_Thread[newID] ) {
        for ( int p = 0; p < pre_fetch; p++ ) {
            node cur = path.back();
            if ( (cur.x >= N/2 -2 && cur.x <= N/2+1) && (cur.y >= N/2 -2 && cur.y <= N/2+1) )
                break;
            vector<node> options;
            if ( maze[cur.x][cur.y].edge[3] == 0 ) {
                if ( cur.x > 0 ) {
                    if ( visit[newID][cur.x-1][cur.y] == 0 ) {
                        options.push_back ( node (cur.x-1, cur.y) );
                        //cout<<"changing visit"<<endl;
                    }
                }
            }
            if ( maze[cur.x][cur.y].edge[2] == 0 ) {
                if ( cur.y < N-1 ) {
                    if ( visit[newID][cur.x][cur.y+1] == 0 ) {
                        options.push_back ( node (cur.x, cur.y+1) );
                        //cout<<"changing visit"<<endl;
                    }
                }
            }
            if ( maze[cur.x][cur.y].edge[1] == 0 ) {
                if ( cur.x < N-1 ) {
                    if ( visit[newID][cur.x+1][cur.y] == 0 ) {
                        options.push_back ( node (cur.x+1, cur.y) );
                        //cout<<"changing visit"<<endl;
                    }
                }
            }
            if ( maze[cur.x][cur.y].edge[0] == 0 ) {
                if ( cur.y > 0 ) {
                    if ( visit[newID][cur.x][cur.y-1] == 0 ) {
                        options.push_back ( node (cur.x, cur.y-1) );
                        //cout<<"changing visit"<<endl;
                    }
                }
            }
            int deltax;
            int deltay;
            if ( options.size() != 0 ) {
                int select = rand()%(options.size());
                path.push_back ( node ( options[select].x, options[select].y ) );
                visit[newID][options[select].x][options[select].y] = 1;
            }
            else {
                path.pop_back();
                p--;
            }
        }
    }
    int deltax;
    int deltay;
    int iter = 0;
    while ( AI_Thread[newID]  ) {
        //cout << iter << " " << path.size () << endl;
        if ( playerFreeze[newID] == 0 ) {
            vector<int> shoot_select;
            for ( int i = 0; i < players; i++ ) {
                if ( i == newID )
                    continue;
                if ( line_of_sight ( otherPlayers[newID], otherPlayers[i] ) ) {
                    shoot_select.push_back(i);
                }

            }
            if ( shoot_select.size () != 0 ) {
                int i = shoot_select[rand()%shoot_select.size()];
                float netx1 = edgeLength/2 + edgeLength*otherPlayers[newID].gridx + otherPlayers[newID].x;
                float nety1 = edgeLength/2 + edgeLength*otherPlayers[newID].gridy + otherPlayers[newID].y;
                float netx2 = edgeLength/2 + edgeLength*otherPlayers[i].gridx + otherPlayers[i].x;
                float nety2 = edgeLength/2 + edgeLength*otherPlayers[i].gridy + otherPlayers[i].y;
                float sq = sqrt ( pow ( netx2 - netx1, 2) + pow ( nety2 - nety1, 2 ) );
                prevlx = (netx2-netx1)/sq;;
                prevly = (nety2-nety1)/sq;
                prevgridx = otherPlayers[newID].gridx;
                prevgridy = otherPlayers[newID].gridy;
                prevx = otherPlayers[newID].x;
                prevy = otherPlayers[newID].y;
                //cout << newID << " " << " " << lx << " " << ly << " ";
                pthread_t t;
                pthread_create ( &t, NULL, shoot_ai, (void*)newID );
            }
            deltax = path[iter+1].x - path[iter].x;
            deltay = path[iter+1].y - path[iter].y;
            int move_num = edgeLength*100;
            for ( int i = 0; i < move_num; i++ ) {
                otherPlayers[newID].x += deltax*0.01;
                otherPlayers[newID].y += deltay*0.01;
                if ( otherPlayers[newID].x > +edgeLength/2 ) {
                    otherPlayers[newID].x += -edgeLength;
                    otherPlayers[newID].gridx++;
                }
                if ( otherPlayers[newID].x < -edgeLength/2 ) {
                    otherPlayers[newID].x += +edgeLength;
                    otherPlayers[newID].gridx--;
                }
                if ( otherPlayers[newID].y > +edgeLength/2 ) {
                    otherPlayers[newID].y += -edgeLength;
                    otherPlayers[newID].gridy++;
                }
                if ( otherPlayers[newID].y < -edgeLength/2 ) {
                    otherPlayers[newID].y += +edgeLength;
                    otherPlayers[newID].gridy--;
                }
                usleep ( 7500 );
            }
            iter++;
            if ( iter == path.size() - 1 )
                break;
        }
    }
    circle_around ( newID );
    AI_Thread[newID] = true;
}

void spawn_AI(int i){
    //visit[i] = new bool*[N];
    for(int j =0; j<N; j++){
        //visit[i][j] = new bool[N];
        for(int k = 0; k<N;k++){
            visit[i][j][k] = 0;
        }
    }
    otherPlayers[i].ai = true;
    evictrandomly(i);
    //cout << "Evict Randomly" << endl;
    pthread_t t1;
    //cout << "New Thread Created With" << otherPlayers[i].gridx <<"  " << otherPlayers[i].gridy << endl;
    pthread_create ( &t1, NULL, depthfirst, (void*)i );
    display_player[i] = true;
    pthread_detach(t1);
}

void convert_AI(int i){
    //visit[i] = new bool*[N];
    for(int j =0; j<N; j++){
        //visit[i][j] = new bool[N];
        for(int k = 0; k<N;k++){
            visit[i][j][k] = 0;
        }
    }
    //evictrandomly(i);
    otherPlayers[i].ai = true;
    //cout << "Evict Randomly" << endl;
    pthread_t t1;
    //cout << "New Thread Created With" << otherPlayers[i].gridx <<"  " << otherPlayers[i].gridy << endl;
    pthread_create ( &t1, NULL, depthfirst, (void*)i );
    display_player[i] = true;
    pthread_detach(t1);
}

void draw_background(){
    glBegin(GL_QUADS);
    glTexCoord3d(0.0,0.0, -1); glVertex3f(-global_w, -global_h, -global_h);
    glTexCoord3d(1.0,0.0, -1); glVertex3f(global_w, -global_h, -global_h);
    glTexCoord3d(1.0,1.0, -1); glVertex3f(global_w, global_h, -global_h);
    glTexCoord3d(0.0,1.0, -1); glVertex3f(-global_w, global_h, -global_h);
    glEnd();
}

void *winner_countdown ( void* id ) {
    //time_left = 1000;
    while ( count_down ) {
        usleep ( 20000 );
        //cout << time_left << endl;
        time_left--;
        if ( count_down == 0 )
            break;
        if ( time_left == 0 ) {
            connection.Stop();
            if(orbs[0] == mynumber ) { 
                DispScreen = 1;
                usleep(5000000);
                exit(0);
            }
            else 
                DispScreen = 2;
                usleep(5000000);
                exit(0);
            //exit(0);
        }
    }
}

void drawTrophy() {

    //gluQuadricTexture ( obj, GLU_TRUE);
    gluCylinder(obj1, 0.15, 0.15, 0.075, 20, 20);
    glTranslatef(0.0f, 0.0f, 0.075f);
    glColor3f(0.855f, 0.647f, 0.125f);
    gluCylinder(obj3, 0.0375, 0.0375, 0.15, 40, 40 );
    glTranslatef(0.0f, 0.0f, 0.15f);
    gluCylinder(obj2, 0.0375, 0.3, 0.525, 20, 20);
    glTranslatef(0.0, 0.0, 0.15);
    glRotatef(90,1,0,0);
    glutSolidTorus( 0.03, 0.18, 100, 100);
    glRotatef(-90,1,0,0);
    //glRotatef(0,0,0,1);
    //glLoadIdentity();

}

void display() {
    if (DispScreen == 0){
        if ( orbs[0] != 6 && ( orbs[0] == orbs[1] ) && ( orbs[1] == orbs[2] ) && ( orbs[2] == orbs[3] ) && count_down == 0 ) {
            //cout << "starting" << endl;
            count_down = 1;
            pthread_t win;
            pthread_create ( &win, NULL, winner_countdown, (void*)0 );
        }

        if ( orbs[0] != orbs[1] || orbs[1] != orbs[2] || orbs[2] != orbs[3] ) {
            count_down = 0;
            if(time_left < 700)
            time_left+=2;
        }

    	for(int i = 0 ; i < players ; i++ ) {
    		if(playerFreeze[i] > 0)
    			playerFreeze[i]--;
    		else
    			playerFreeze[i]==0;
    	}

        updatepowerUps((0.03), 20);
        mouseWarp = !mouseWarp;
        //glClearColor( 0, 0, 0, 1 );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, 1.6, 0.01, 1000.0);
        gluLookAt ( edgeLength/2 + otherPlayers[mynumber].gridx*edgeLength + otherPlayers[mynumber].x - lx*2*edgeLength, edgeLength/2 + otherPlayers[mynumber].gridy*edgeLength + otherPlayers[mynumber].y - ly*2*edgeLength, 1.95*mazeHeight, 
                    edgeLength/2 + otherPlayers[mynumber].gridx*edgeLength + otherPlayers[mynumber].x, edgeLength/2 + otherPlayers[mynumber].gridy*edgeLength + otherPlayers[mynumber].y, 1.25*mazeHeight,
                    0, 0, 1 );
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        //glRotatef(_angle, 0.0f, 1.0f, 0.0f);
        //Set the colour here
        //glColor3f(1.0f, 0.0f, 0.0f);
        //glScalef(1, -1, 1);
        //glDisable(GL_LIGHTING);
        
        
        // glDisable( GL_TEXTURE_2D );
        glEnable( GL_TEXTURE_2D );
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindTexture( GL_TEXTURE_2D, texture[0] );
        drawCube ( -wallThickness/2, -wallThickness/2, (N)*edgeLength + wallThickness/2, (N)*edgeLength + wallThickness/2, -0.1);
        
        //glEnable(GL_DEPTH_TEST);   
        //glEnable(GL_BLEND);                 // Turn Blending On
       // glDisable(GL_DEPTH_TEST);           // Turn Depth Testing Off
        //glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        glLoadIdentity();   // Blend to Make The Cube Transparent

        glTranslatef ( (N/2)*edgeLength, (N/2)*edgeLength, 0.0f );
        glDisable(GL_TEXTURE_2D);
        //glTranslatef((N/2)*edgeLength, (N/2)*edgeLength, 0.0f);
        glEnable(GL_BLEND);                 // Turn Blending On
        glDisable(GL_DEPTH_TEST);           // Turn Depth Testing Off
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        glColor3f(0.0f, 1.0f, 1.0f);
        GLUquadricObj *obj = gluNewQuadric();
        //gluQuadricTexture ( obj, GLU_TRUE);
        gluCylinder(obj, 0.2, 0.2, 10, 100, 100);
        glColor3f ( 1.0, 1.0, 1.0 );
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_ONE, GL_ONE);
        glTranslatef(0,0, 0.01*time_left);
        drawTrophy();
        //glTranslatef(0.0, 0.0, -0.01*time_left);

        glLoadIdentity();
        glTranslatef ( (N/2)*edgeLength, (N/2)*edgeLength, 0.0f );
        //glColor3f(1.0f, 1.0f, 1.0f);

        glTranslatef ( 0, -0.6, 2*playerRadius );
        glColor3f( PlayerColour[orbs[0]][0], PlayerColour[orbs[0]][1], PlayerColour[orbs[0]][2] );
        glutSolidSphere ( 0.15, 20, 20 );
        glTranslatef ( 0, +0.6, 0 );
        glColor3f ( 1.0, 1.0, 1.0 );

        glTranslatef ( 0, +0.6, 0 );
        glColor3f( PlayerColour[orbs[2]][0], PlayerColour[orbs[2]][1], PlayerColour[orbs[2]][2] );
        glutSolidSphere ( 0.15, 20, 20 );
        glTranslatef ( 0, -0.6, 0 );
        glColor3f ( 1.0, 1.0, 1.0 );

        glTranslatef ( -0.6, 0, 0 );
        glColor3f( PlayerColour[orbs[3]][0], PlayerColour[orbs[3]][1], PlayerColour[orbs[3]][2] );
        glutSolidSphere ( 0.15, 20, 20 );
        glTranslatef ( +0.6, 0, 0 );
        glColor3f ( 1.0, 1.0, 1.0 );

        glTranslatef ( +0.6, 0, 0 );
        glColor3f( PlayerColour[orbs[1]][0], PlayerColour[orbs[1]][1], PlayerColour[orbs[1]][2] );
        glutSolidSphere ( 0.15, 20, 20 );
        glTranslatef ( -0.6, 0, -2*playerRadius );

        glColor3f ( 1.0, 1.0, 1.0 );
        glLoadIdentity();
       // glDisable(GL_BLEND);                // Turn Blending Off
        glDisable(GL_BLEND);                 // Turn Blending On
        glEnable(GL_DEPTH_TEST);           // Turn Depth Testing Off
        glBlendFunc(GL_ZERO,GL_ONE);     
        //glBlendFunc(GL_ZERO,GL_ONE);        // Turn Depth Testing Off
         glEnable(GL_TEXTURE_2D);
        
        //Enable Lighting
        //glMultMatrixf(view_rotate);
        //cout << maze[0][0].edge[0] << " " << maze[0][0].edge[1] << " " << maze[0][0].edge[2] << " " << maze[0][0].edge[3] << endl;
        for ( int i = 0; i < N; i++ ) {
            for ( int j = 0; j < N; j++ ) {
            	glLoadIdentity();
                glTranslatef ( edgeLength/2 + i*edgeLength, edgeLength/2 + j*edgeLength, 0.0 );
                if ( maze[i][j].edge[0] )
                    drawCube ( -(edgeLength-wallThickness)/2, -(edgeLength+wallThickness)/2, +(edgeLength-wallThickness)/2, -(edgeLength-wallThickness)/2, mazeHeight);
                if ( maze[i][j].edge[3] && i == 0 )
                    drawCube ( -(edgeLength+wallThickness)/2, -(edgeLength-wallThickness)/2, -(edgeLength-wallThickness)/2, +(edgeLength-wallThickness)/2, mazeHeight);
                if ( maze[i][j].edge[2] && j == N-1 )
                    drawCube ( -(edgeLength-wallThickness)/2, +(edgeLength+wallThickness)/2, +(edgeLength-wallThickness)/2, +(edgeLength-wallThickness)/2, mazeHeight);
                if ( maze[i][j].edge[1] )
                    drawCube ( +(edgeLength+wallThickness)/2, -(edgeLength-wallThickness)/2, +(edgeLength-wallThickness)/2, +(edgeLength-wallThickness)/2, mazeHeight);
               // glTranslatef ( -(edgeLength/2 + i*edgeLength), -(edgeLength/2 + j*edgeLength), 0.0 );
            }
        }
      for ( int i = 0; i <= N; i++ ) {
        for ( int j = 0; j <= N; j++ ) {
        	if ( i < N && j < N ) {
                if( maze[i][j].edge[0] || maze[i][j].edge[3] ){
                            glLoadIdentity();
            
                              glTranslatef ( i*edgeLength, j*edgeLength, 0.0 );
                 
                              drawCube ( -wallThickness/2, -wallThickness/2, +wallThickness/2, +wallThickness/2, mazeHeight );
                              continue;
                 }
            }
            if ( i > 0 && j > 0 ) {
                if ( maze[i-1][j-1].edge[1] || maze[i-1][j-1].edge[2] ) {
                    glLoadIdentity();

                    glTranslatef ( i*edgeLength, j*edgeLength, 0.0 );
         
                    drawCube ( -wallThickness/2, -wallThickness/2, +wallThickness/2, +wallThickness/2, mazeHeight );
                }
            }
        }
       }
            if(powerUpangle<360)
            powerUpangle+=4;
        else
            powerUpangle = -360;
        for(int i = 0 ; i < numpowerUp ; i ++ ) {
            glBindTexture( GL_TEXTURE_2D, texture[2 + (powerUps[i].type)] );
            if (powerUps[i].exist){
            glLoadIdentity();
            
            glTranslatef ( edgeLength/2 + powerUps[i].gridx*edgeLength, edgeLength/2 + powerUps[i].gridy*edgeLength, 2*playerRadius );
               glPushMatrix();
            glRotatef(powerUpangle, 0,0 ,1 );
            drawCube(-1*(powerUpCubeLength/2),-1*(powerUpCubeLength/2), powerUpCubeLength/2, powerUpCubeLength/2, powerUpCubeLength);
            glPopMatrix();

            }
        }
    	//glDisable(GL_BLEND);
    	//glEnable(GL_DEPTH_TEST);
        glDisable( GL_TEXTURE_2D );
        glLoadIdentity();
        check_coords ();
        glLoadIdentity();
        
        for (int i = 0 ; i < players ; i++){
            if (display_player[i]){
            	if (mystat[i].hit > 0){
            		mystat[i].hit--;
            		glColor3f(1.0,0.0,0.0);
            	}
            	    glTranslatef ( edgeLength/2 + otherPlayers[i].gridx*edgeLength, edgeLength/2 + otherPlayers[i].gridy*edgeLength, 2*playerRadius );
        			glTranslatef ( otherPlayers[i].x, otherPlayers[i].y, 0.0 );
        			if(playerFreeze[i] == 0){
    	    			if( i == mynumber )
    	    				glColor3f(SpellColour[SpellID][0], SpellColour[SpellID][1], SpellColour[SpellID][2] );
    	    			else
    	    				glColor3f( PlayerColour[i][0], PlayerColour[i][1], PlayerColour[i][2] );
    	    		}
    	    		else 
    	    			glColor3f(1,1,1);
        			glutSolidSphere ( playerRadius*.4, 40, 40 );
        			if(playerFreeze[i] == 0)
        				glColor3f( PlayerColour[i][0], PlayerColour[i][1], PlayerColour[i][2] );
        			else 
        				glColor3f(1,1,1);
            		if (mystat[i].shield > 0){
           				 glutWireSphere(playerRadius*1.1,40,40);
        			}
        			glPushMatrix();
       			 	glRotatef(torangle + 90, 1,0,0 );
        			glutSolidTorus( playerRadius*0.1,
                       	playerRadius,
                       	100, 100);
        			glPopMatrix();
        			glPushMatrix();
    			    glRotatef(torangle, 0,1 ,0 );
    			    glutSolidTorus( playerRadius*0.1,
    			                   playerRadius,
    			                   100, 100);
    			    glColor3f(1.0f, 0.0f, 0.0f);
    			    glPopMatrix();
    			    glTranslatef (0, 0, 2*playerRadius);
    			    //glRotatef(lx, 0,0,1);
    			    drawCube((-1)*playerRadius, -0.03, (-1)*playerRadius + (2*mystat[i].health*playerRadius)/(HEALTHMAX), 0.03, 0.03);
    			    glPushMatrix();
    			    HealthAngle = (int)atan((-1)*lx/ly);
    			    glRotatef(HealthAngle, 0 , 0 , 1 );
    			    glColor3f(0 , 0, 1);
    			    glTranslatef (0, 0, 0.07);
    			    drawCube((-1)*playerRadius, -0.03, (-1)*playerRadius + (2*mystat[i].mana*playerRadius)/(MANAMAX), 0.03, 0.03);
    			    glPopMatrix();
    			    glColor3f(1.0f, 1.0f, 1.0f);
    			    glPopMatrix();
    			    glTranslatef ( -otherPlayers[i].x, -otherPlayers[i].y, 0.0 );
    			    glTranslatef ( -1*(edgeLength/2 + otherPlayers[i].gridx*edgeLength), -1*(edgeLength/2 + otherPlayers[i].gridy*edgeLength), -2*playerRadius );
    			    glLoadIdentity();
                glColor3f(1.0f, 1.0f, 1.0f);
            }
            else{
                if (AI_Thread[i]){
                    spawn_AI(i);
                            //create New AI thread with old ID.
                }


            }
        }
                if(torangle<360)
                    torangle+=3;
                else
                    torangle = -360;
        int bulletLoop = 0;
        GLfloat redDiffuseMaterial[] = {1.0, 0.0, 0.0};
        GLfloat whiteSpecularMaterial[] = {0.0, 0.0,0.0};
        GLfloat greenEmissiveMaterial[] = {0.0, 1.0, 0.0};
        GLfloat mShininess[] = {128};
        
        
        while ( bulletLoop < spell_on_screen.size () ) {
            if ( spell_on_screen[bulletLoop]->exist == 1 ) {
                glTranslatef ( edgeLength/2 + spell_on_screen[bulletLoop]->gridx*edgeLength, edgeLength/2 + spell_on_screen[bulletLoop]->gridy*edgeLength, 2*playerRadius );
                glTranslatef ( spell_on_screen[bulletLoop]->x, spell_on_screen[bulletLoop]->y, 0.0 );
                glColor3f(SpellColour[spell_on_screen[bulletLoop]->type][0], SpellColour[spell_on_screen[bulletLoop]->type][1],SpellColour[spell_on_screen[bulletLoop]->type][2] );
                //glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteSpecularMaterial);
                //glMaterialfv(GL_FRONT, GL_SHININESS, mShininess);
                glutSolidSphere ( bulletRadius, 10, 10 );
                //glTranslatef ( -spell_on_screen[bulletLoop]->x, -spell_on_screen[bulletLoop]->y, 0.0 );
                //glTranslatef ( -1*(edgeLength/2 + spell_on_screen[bulletLoop]->gridx*edgeLength), -1*(edgeLength/2 + spell_on_screen[bulletLoop]->gridy*edgeLength), -2*playerRadius );
            	glColor3f(1.0f, 1.0f, 1.0f);
            	glLoadIdentity();
            }
            else {
                spell_on_screen.erase( spell_on_screen.begin() + bulletLoop );+
                ToalBullets++;
                //cout<<"TOTAL BULLETS "<<ToalBullets<<endl;
                bulletLoop--;
            }
            
            bulletLoop++;
        }
        glDisable(GL_BLEND);                // Turn Blending Off
        glEnable(GL_DEPTH_TEST);            // Turn Depth Testing Off
        glEnable( GL_TEXTURE_2D );
        glEnable(GL_LIGHTING);
        glFlush();
        glutSwapBuffers();
    }
    else if (DispScreen == 1 ) {
        glLoadIdentity();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable( GL_TEXTURE_2D );
        glDisable(GL_LIGHTING);                     //Disable Lighting to Draw Textured Objects
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(-1920, 1920, -1080, 1080, -5500.0, 5500.0);
        glScalef(1, -1, 1);                         //Reivert Along Y-axis to amke
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glBindTexture( GL_TEXTURE_2D, texture[5] );  //Load texture According to the input from the GLUI
        draw_background();                                          //Draw Background
        
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glLoadIdentity();
        glFlush();
        glutSwapBuffers();
    }
    else if (DispScreen == 2) {
        glLoadIdentity();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable( GL_TEXTURE_2D );
        glDisable(GL_LIGHTING);                     //Disable Lighting to Draw Textured Objects
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(-1920, 1920, -1080, 1080, -5500.0, 5500.0);
        glScalef(1, -1, 1);                         //Reivert Along Y-axis to amke
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glBindTexture( GL_TEXTURE_2D, texture[6] );  //Load texture According to the input from the GLUI
        draw_background();                                          //Draw Background
        
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glLoadIdentity();
        glFlush();
        glutSwapBuffers();
    }

    else {
        glLoadIdentity();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable( GL_TEXTURE_2D );
        glDisable(GL_LIGHTING);                     //Disable Lighting to Draw Textured Objects
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(-1920, 1920, -1080, 1080, -5500.0, 5500.0);
        glScalef(1, -1, 1);                         //Reivert Along Y-axis to amke
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glBindTexture( GL_TEXTURE_2D, texture[7] );  //Load texture According to the input from the GLUI
        draw_background();                                          //Draw Background
        
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glLoadIdentity();
        glFlush();
        glutSwapBuffers();
    }
}

void timer(int v) {
    _angle += 0.0f;
    if (_angle > 360) {
        _angle -= 360;
    }
    if (1) {
        glutPostRedisplay();
    }
    glutTimerFunc(30, timer, v);
}

void myGlutIdle( void ) {                           //Idle Function
    glutSetWindow(main_window);
    glutPostRedisplay();
}

void keyboard(int key, int x, int y){
	if(playerFreeze[mynumber] == 0) {
	    switch (key) {
	        case GLUT_KEY_LEFT :
	            otherPlayers[mynumber].x -= 0.1*ly;
	            otherPlayers[mynumber].y += 0.1*lx;
	            break;
	        case GLUT_KEY_RIGHT :
	            otherPlayers[mynumber].x += 0.1*ly;
	            otherPlayers[mynumber].y -= 0.1*lx;
	            break;
	        case GLUT_KEY_UP :
	            //keyup = true;
	            otherPlayers[mynumber].x += 0.1*lx;
	            otherPlayers[mynumber].y += 0.1*ly;
	            break;
	        case GLUT_KEY_DOWN :
	            otherPlayers[mynumber].x += -0.1*lx;
	            otherPlayers[mynumber].y += -0.1*ly;
	            break;
		}
    }
}

void *shootrecieve ( void *id ) {
    spell *temp_rec;
    temp_rec = new spell;
    //temp_rec->type = SpellSelected;

    temp_rec = &tempspellRec;
    temp_rec->my_shooter = tempspellRec.my_shooter;
    temp_rec->exist = 1;
    RecSpellCollect.pop_back();
    //cout<<RecSpellCollect.size()<<" SIZE "<<endl;
    //PendingBulletsSend.push_back (temp_rec);
    spell_on_screen.push_back ( temp_rec );
    while ( 1 ) {
        temp_rec->exist = check_coords_bullet ( temp_rec );
        if ( temp_rec->exist == 0 ) {
            break;
        }
        temp_rec->x += temp_rec->lx*0.1;
        //cout << temp_rec->x << endl;
        temp_rec->y += temp_rec->ly*0.1;
        //cout<<"yoyo"<<endl;
        usleep (10000);
        //cout<<"yoyo1"<<endl;
    }
    //usleep (10000);
    //cout<<"exit"<<endl;
    //pthread_exit ( NULL );
}


void bullet ( int button, int state, int x, int y) {
    if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && playerFreeze[mynumber] == 0) {
        prevlx = lx;
        prevly = ly;
        prevgridx = otherPlayers[mynumber].gridx;
        prevgridy = otherPlayers[mynumber].gridy;
        prevx = otherPlayers[mynumber].x;
        prevy = otherPlayers[mynumber].y;
        //cout << mynumber << " " << SpellID << " " << lx << " " << ly << endl;
    	if(SpellID == 0 && mystat[mynumber].mana >= 20) {
            mystat[mynumber].mana -= 20 ;
            pthread_t t;
	        pthread_create ( &t, NULL, shoot, (void*)SpellID );
    	}
        else if (SpellID == 1 && mystat[mynumber].mana >= 1) {
            mystat[mynumber].mana -= 1 ;
            pthread_t t;
	        pthread_create ( &t, NULL, shoot, (void*)SpellID );
        }
        else if (SpellID == 2 && mystat[mynumber].mana >= 5) {
            mystat[mynumber].mana -= 5;
            pthread_t t;
	        pthread_create ( &t, NULL, shoot, (void*)SpellID );
            		//tempspellRec = new spell;
        }
    }
    if (  button == GLUT_RIGHT_BUTTON && state == GLUT_UP ) {
    	SpellID = (SpellID + 1)%3;
    }
}
    
void mouseMove(int x, int y) {

  // this will only be true when the left button is down

    // update deltaAngle
  if (mouseWarp){
    deltaAngle = -(x - 800) * 0.001f;
    // update camera's direction
    lx = -sin(angle + deltaAngle);
    ly = cos(angle + deltaAngle);
    angle += deltaAngle;
      glutWarpPointer(800, 450);
    mouseWarp = !mouseWarp;
   // xOrigin = x;
//}
}
}

void *maindisplay ( void *id ) {
    //int* intid = (int*) id;
    for (int i = 0 ; i < PLAYERMAX; i++ ){
    	//cout<<"reaching here "<<endl;
        //visit[i] = new bool*[N];
        for(int j =0; j< N; j++){
        //visit[i][j] = new bool[N];
            for(int k = 0; k< N;k++){
                visit[i][j][k] = 0;
            }
        }
    }

    if (gamestyle == Single){
        for (int i = 1 ; i < players ; i++){
                //srand ( time(NULL));
                //int ***visit = new int**[PLAYERAX];
                spawn_AI(i);
                //initilize AI's
                //Add threadno to AIThread
        }
    }
    else if (mode == Host){
        for (int i = networkplayers ; i < players ; i++ ){
            spawn_AI(i);
        }
    }

    glutInit(&c, v);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(80, 80);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("Labyrinth");
    initRendering();
   // window_id =
    glutFullScreen();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutTimerFunc(100, timer, 0);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutMouseFunc(bullet);
    glutPassiveMotionFunc(mouseMove);
    glutSpecialFunc(keyboard);
    //cout<<"potty"<<endl;
    glutMainLoop();
}

/*void *increment ( void *id ) {
 while (1){
 //cout << "hello" << endl;
 if ( rec_pos.gridx != otherPlayers.gridx || rec_pos.gridy != otherPlayers.gridy ) {
 otherPlayers = rec_pos;
 //sleep(1000/30);
 }
 else {
 //cout << rec_pos.x << endl;
 lx2 = (rec_pos.x - otherPlayers.x)/100;
 ly2 = (rec_pos.y - otherPlayers.y)/100;
 for ( int i = 0; i < 100; i++ ) {
 otherPlayers.x += lx2;
 otherPlayers.y += ly2;
 usleep(10000/30);
 }
 
 }
 }
 }*/




int main( int argc, char * argv[] ){
    DispScreen = 0;
    for (int i = 0 ; i < PLAYERMAX ; i++){
        display_player[i] = true;
        AI_Thread[i] = true;
    }
    //cout << sizeof(pos) << endl;
    srand (time(NULL));
    enum GameMode{
        On,
        Off,
        Initializing1,
        Initializing2,
        Initializing3,
        Initialized
    };
    mode = Host;
    
    GameMode gamemode = Off;
    Address address[4];
    players = 1;
    int port;
    
    c = argc;
    v = argv;

    gamestyle = Multi;
    if (argc == 2){
        players += atoi(argv[1]) ;
        gamestyle = Single;
    }
    else if ( argc == 4 ) // Player
    {
        int a,b,c,d;
        if ( sscanf( argv[1], "%d.%d.%d.%d", &a, &b, &c, &d ) )
        {
            mode = Player;
            address[0] = Address(a,b,c,d,atoi(argv[2]));
            port = atoi(argv[3]);
        }
    }
    else if (argc == 5){
        port = atoi(argv[1]);
        address[0] = Address(127,0,0,1,atoi(argv[1]));
        networkplayers = atoi(argv[2]);
        AI_number = atoi(argv[3]);
    }
    //NumPlayers = players;

    
    if (gamestyle == Single) {
        connection.dum_Stop();
            mynumber = 0;
            networkplayers = 0 ;
            maze = new grid*[N];
            for ( int i = 0; i < N; i++ )
                maze[i] = new grid[N];
            initialize ( maze, N );
            prims ( maze, N );
            initializePowerups();
            for (int i = 0 ; i < players ; i++){
                evictrandomly(i);
            }
            mynumber = 0;
    }
    else{
        if ( !connection.Start( port ) ){
            printf( "could not start connection on port %d\n", port );
            return 1;
        }
        struct Addrs{
        Address add;
        bool status;
        };
        struct datasend{
        int networkplayers; 
        int i;
        int j;
        Address address1;
        Address address2;
        Address address3;
        };
        while (gamemode != On ){
            if(mode == Host){
                //cout << "yoooooooo" << endl;
                mynumber = 0;
                maze = new grid*[N];
                for ( int i = 0; i < N; i++ )
                    maze[i] = new grid[N];
                unsigned char temp[256];
                for ( int i = 0; i < 256; i++ )
                    temp[i] = 0;

                initialize ( maze, N );
                prims ( maze, N );
                printf("Waiting for Player to Connect\n");
                while (gamemode == Off){
                    unsigned char packet[512];
                    Address temp;
                    int bytes_read = connection.Receive_Packet( packet, sizeof(packet) ,temp, 0x11223338);
                    if (bytes_read != 0){
                        bool f=false;
                        for (int i = 0 ; i < players ; i++ ){
                            if (i != mynumber){
                                if (temp == address[i]){
                                    f=true;
                                    break;
                                }
                            }
                        }
                        if (!f){
                            address[players] = temp;
                            players++;
                            printf("New Player\n");
                        }
                        if (players == networkplayers){
                            gamemode = Initializing1;
                        }
                    }
                }
                Addrs temporary[3];
                for (int i = 0; i < players-1 ; i++){
                    temporary[i].add = address[i+1];
                    temporary[i].status = false;
                }
                printf("Server :Send Maze Data\n");
                while (gamemode == Initializing1){
                    
                    unsigned char packet[PacketSize];
                    memset( packet, 0, sizeof( packet ) );
                    maze_to_packet ( maze, packet );
                    for (int i = 1 ; i < players ; i++){
                        
                        connection.Send_Packet( packet, sizeof( packet ) ,address[i],0x11223339);
                    }
                    memset( packet, 0, sizeof( packet ) );
                    Address temp;
                    //send maze data.
                    int bytes_read = connection.Receive_Packet( packet, sizeof(packet), temp,0x11223340 );
                    if ( bytes_read != 0 ){
                        for (int i = 0 ; i < players - 1 ; i++ ){
                            if (!temporary[i].status && temporary[i].add == temp){
                                temporary[i].status = true;
                            }
                        }
                        bool r =true;
                        for (int i = 0 ; i < players - 1 ; i++ ){
                            if (!temporary[i].status ){
                                r = false;
                                break;
                            }
                        }
                        if (r){
                            gamemode = Initializing2;
                            printf("Server :Received Confirmation for Maze\n");
                        }
                    }
                }
                for (int i = 0; i < players-1 ; i++){
                    temporary[i].status = false;
                }
                printf("Server :Send all the addresses with blanking the address of the player itself.\n");
                while (gamemode == Initializing2){
                    for (int i = 1 ; i < players ; i++){
                        datasend send;
                        send.i = players + AI_number;
                        send.j = i ;
                        send.networkplayers = networkplayers;
                        send.address1 = address[1];
                        send.address2 = address[2];
                        send.address3 = address[3];
                        
                        connection.Send_Packet( (unsigned char*)&send, sizeof( send ) ,address[i],0x11223341);
                    }
                    unsigned char packet[PacketSize];
                    memset( packet, 0, sizeof( packet ) );
                    Address temp;
                    //send maze data.
                    int bytes_read = connection.Receive_Packet( packet, sizeof(packet), temp,0x11223342 );
                    if ( bytes_read != 0 ){
                       // printf("Server :Received Codfgggdfon for Player Addresses\n");
                        for (int i = 0 ; i < players - 1 ; i++ ){
                            if (!temporary[i].status && temporary[i].add == temp){
                                printf(":: %d \n",i);
                                temporary[i].status = true;
                            }
                        }
                        bool r =true;
                        for (int i = 0 ; i < players - 1 ; i++ ){
                            if (!temporary[i].status ){
                                r = false;
                                break;
                            }
                        }
                        if (r){
                            gamemode = Initializing3;
                           // printf("Server :Received Confirmation for Player Addresses\n");
                        }
                    }
                }
                for (int i = 0; i < players-1 ; i++){
                    temporary[i].status = false;
                }
                initializePowerups();
                printf("Server :Send all the Power Ups.\n");
                while (gamemode == Initializing3){
                    
                    for (int i = 1 ; i < players ; i++){
                        connection.Send_Packet( (unsigned char*)&powerUps, sizeof( powerUps ) ,address[i],0x11223345);
                    }
                    unsigned char packet[PacketSize];
                    memset( packet, 0, sizeof( packet ) );
                    Address temp;
                    //send maze data.
                    int bytes_read = connection.Receive_Packet( packet, sizeof(packet), temp,0x11223346 );
                    if ( bytes_read != 0 ){
                       // printf (" Sasdfsasadasd :: %d \n", temp.GetPort()) ;
                        for (int i = 0 ; i < players - 1 ; i++ ){
                            if (!temporary[i].status && temporary[i].add == temp){
                                temporary[i].status = true;
                            }
                        }
                        bool r =true;
                        for (int i = 0 ; i < players - 1 ; i++ ){
                            if (!temporary[i].status ){
                                r = false;
                                break;
                            }
                        }
                        if (r){
                            gamemode = Initialized;
                            printf("Server :Received Confirmation for Power Ups\n");
                        }
                    }
                }
                while (gamemode == Initialized){
                    for (int j = 1 ; j < players ; j++){
                        for (int i = 0 ; i< 20;i++){
                            unsigned char packet[PacketSize];//send command to start the game to start
                            memset( packet, 0, sizeof( packet ) );
                            connection.Send_Packet( packet, sizeof( packet ),address[j] ,ProtocolId);
                            //usleep
                        }
                    }
                    printf("Server :Start the game\n");
                    players = networkplayers + AI_number;
                    gamemode = On;
                }
            }
            else{
            printf("Client :trying to connect\n");
            for (int i = 0 ; i < 20000 ; i++){
                unsigned char packet[PacketSize];
                connection.Send_Packet( packet, sizeof(packet),address[0],0x11223338 );
                Address temp;
                int bytes_read = connection.Receive_Packet( packet, sizeof(packet),temp,0x11223339 );
                if ( bytes_read != 0 && temp == address[0]){
                    //received maze data!!
                    maze = new grid*[N];
                    for ( int i = 0; i < N; i++ )
                        maze[i] = new grid[N];
                    otherPlayers[mynumber].x = otherPlayers[mynumber].y = 0;
                    otherPlayers[mynumber].gridx = otherPlayers[mynumber].gridy = 0;
                    initialize ( maze, N );
                    packet_to_maze ( packet, maze );
                    printf( "client is connected to server,received maze data\n" );
                    gamemode = Initializing1;
                    break;
                }
                usleep(1000);
                i++;
                if ( i == 19999){
                    printf("HOST not found \n");
                    exit(0);
                }

            }
            printf("Client :Send confirmation for Mazes\n");
            while (gamemode == Initializing1){
                unsigned char packet[512];
                connection.Send_Packet( packet, sizeof(packet),address[0],0x11223340 );
                Address temp ;
                int bytes_read = connection.Receive_Packet( packet, sizeof(packet) , temp,0x11223341);
                if ( bytes_read != 0 && temp == address[0] ){
                    datasend *recData;
                    recData = new datasend;
                    recData = (datasend*)&packet;
                    //delete recData;
                    players = recData->i;
                    mynumber = recData->j;
                    networkplayers = recData->networkplayers;
                    address[1] = recData->address1;
                    address[2] = recData->address2;
                    address[3] = recData->address3;
                    printf( "client is connected to server,received Addresses\n" );
                    gamemode = Initializing2;
                }
            }
            printf("Client :Send confirmation for Addreses\n");      
             while (gamemode == Initializing2){
                                    //powerUps = recData;
                unsigned char packet1[512];
                connection.Send_Packet( packet1, sizeof( packet1 ) , address[0],0x11223342);
                unsigned char packet[512];
                Address temp;
                int bytes_read = connection.Receive_Packet( packet, sizeof(packet) , temp,0x11223345);
                if ( bytes_read != 0 && temp == address[0] ){
                    powerUp* recData;
                    recData = new powerUp[numpowerUp];
                    recData = (powerUp*)&packet;
                    //delete recData;
                    for(int i = 0 ; i < numpowerUp ; i ++) {
                        powerUps[i] = recData[i];
                    }
                    printf( "client is connected to server,received powerUps\n" );
                    unsigned char packet[16];
                memset( packet, 0, sizeof( packet ) );
                //for (int i = 0 ; i <10 ; i++ ){

                //}
                gamemode = Initialized;
                }
            }
            printf ("Waiting For Confirmation \n") ;
            while (gamemode == Initialized){

                unsigned char packet[PacketSize];
                memset( packet, 0, sizeof( packet ) );
                //printf("Client :Send confirmation for powerUps\n");
                connection.Send_Packet( packet, sizeof( packet ) , address[0],0x11223346);
                Address temp;
                int bytes_read = connection.Receive_Packet( packet, sizeof(packet), temp ,ProtocolId);
                if ( bytes_read != 0 && temp == address[0] ){
                    printf("Client : Start the game\n");
                    gamemode = On;
                }
            }
            }
        }

    }
    bool connect[4];
    cout<<endl<<"I am "<<mynumber<<endl;
        for (int i = 0; i < networkplayers ; i++){
            connect[i] = true;
            printf("Player Number %d is port %d \n",i,address[i].GetPort());
        }

    for (int i = 0 ; i < players ; i++){
        mystat[i].health = HEALTHMAX;
        mystat[i].mana = MANAMAX;
        mystat[i].shield = 0;
        mystat[i].hit=0;
        otherPlayers[i].my_id = i;
        otherPlayers[i].ai = false;
        display_player[i] = true;
        AI_Thread[i] = true;
    }

    if (gamestyle == Multi){
        evictrandomly(mynumber);
        for (int i = networkplayers ; i < players ; i++){
            otherPlayers[i].ai = true;
            evictrandomly(i);
        }
        pthread_t disp;
        pthread_create ( &disp, NULL, maindisplay, (void*)1 );
        /* pthread_t inc;
        pthread_create ( &inc, NULL, increment, (void*)0 );*/
        connection.receive_addresses(address[0],address[1],address[2],address[3]);
        float sendAccumulator = 0.1f;
        //float statsAccumulator = 0.0f;
        while ( true )
        {
        int stat= 0;
        for (int i = 0; i < networkplayers; i++){
            if ( i != mynumber && connect[i]){
            if (!connection.getstate(i)){
                connect[i] = false;
                if (mode == Host){
                    convert_AI(i);
                }
                else if (mode == Player && (mynumber - i) == 1 ){
                    int host_me = 0;
                    for (int j = 0 ; j < i ; j++){
                        if (connect[j]){
                            host_me ++;
                            break;
                        }
                    }
                    if (host_me == 0){
                        mode = Host;
                        for (int i = networkplayers ; i < players ; i++){
                            convert_AI(i);
                        }
                        for (int i = 0 ; i < networkplayers ; i++){
                            if (connect[i] == false){
                                convert_AI(i);
                            }
                        }
                    }
                }
                printf("player %d disconnected  \n", i);
            }
            else{
                stat += 1;
            }
            }
        }
        if (!stat){
            printf("everyone is disconnected \n");
            connection.Stop();
            DispScreen = 3;
            usleep(5000000);
            exit(0);
            break;
        }
        sendAccumulator += DeltaTime;
        while ( sendAccumulator >  SendRate )
        {
            for ( int k = 0 ; k < PendingBulletsSend.size(); k++){
               	spell temp;
                temp = PendingBulletsSend[k];
                for (int i = 0; i < networkplayers ; i++){
                	if (i!= mynumber && connect[i]){
        				connection.SendPacket((unsigned char*)&temp, sizeof(temp),address[i],ProtocolId,i);
        			}
        		}
        	}
        	PendingBulletsSend.clear();
        	for (int i = 0; i < networkplayers ; i++){
                	if (i!= mynumber && connect[i]){
                    	connection.SendPacket( (unsigned char*)&otherPlayers[mynumber], sizeof( otherPlayers[mynumber] ),address[i],ProtocolId,i );
                        if (mode == Host){
                        for (int k = 0 ; k < players ; k++){
                            if (otherPlayers[k].ai){
                                connection.SendPacket( (unsigned char*)&otherPlayers[k], sizeof( otherPlayers[k] ),address[i],ProtocolId,i );
                            }
                        }
                    }
                }
            }
            if ( mode == Host ) {
                for ( int i = 0; i < networkplayers; i++ ) {
                    if (i!= mynumber && connect[i])
                        connection.SendPacket( (unsigned char*)&orbs, sizeof( orbs ),address[i],ProtocolId,i );
                }
            }

            sendAccumulator -= SendRate;
        }
        while ( true )
        {
            Address addr;
            unsigned char packet[96];
            int i ;
            int bytes_read = connection.ReceivePacket( packet, sizeof(packet),addr,ProtocolId,i);
            if ( bytes_read != 0 ){
            	if (bytes_read == 24){
                pos *recData;
                recData = new pos;
                recData = (pos*)&packet;
                otherPlayers[recData->my_id] = *recData;
                //cout<<"received data "<<recData->my_id;
            	}
            	else if (bytes_read == 36){
            		//cout<<"bulrec"<<endl;
            		spell* temp;
            		temp = new spell;
            		temp = (spell*)&packet;
            		if(temp->type == 0)
            			mystat[temp->my_shooter].mana -= 20 ;
            		else if (temp->type == 1)
            			mystat[temp->my_shooter].mana -= 1;
            		else
            			mystat[temp->my_shooter].mana -= 5;
            		//tempspellRec = new spell;
            		tempspellRec.x = temp->x;
            		tempspellRec.y = temp->y;
            		tempspellRec.gridx = temp->gridx;
            		tempspellRec.gridy = temp->gridy;
            		tempspellRec.lx = temp->lx;
            		tempspellRec.ly = temp->ly;
            		tempspellRec.my_shooter = temp->my_shooter;
            		tempspellRec.type = temp->type;
                     //cout<<"REC"<<temp->x<<" "<<" "<<temp->y<<" "<<" "<<temp->gridx<<" "<<temp->gridy<<" "<<temp->lx<<" "<<temp->ly<<" "<<endl;
                    RecSpellCollect.push_back(tempspellRec);
                    //cout << tempspellRec.my_shooter << " " << tempspellRec.type << " " << tempspellRec.lx << " " << tempspellRec.ly << endl;
            		pthread_t t1;
        			pthread_create ( &t1, NULL, shootrecieve, (void*)0);

            	}
                else if ( bytes_read == 16 ) {
                    int *temp;
                    temp = new int[4];
                    temp = (int*)packet;
                    orbs[0] = temp[0];
                    orbs[1] = temp[1];
                    orbs[2] = temp[2];
                    orbs[3] = temp[3];
                }
            }
            else{
                break;
            }
        }
        
        // update connection
        for (int i = 0; i < networkplayers ; i++){
            if (i!= mynumber && connect[i]){
                connection.Update( DeltaTime ,i);
            }
        }
        
        /*statsAccumulator += DeltaTime;
        
        while ( statsAccumulator >= 0.25f && connection.IsConnected() )
        {
            
            for (int i = 0 ;i < players; i++){
                if (i!= mynumber && connect[i]){
                    float  rtt = connection.GetReliabilitySystem(i).GetRoundTripTime();
                    
                    unsigned int sent_packets = connection.GetReliabilitySystem(i).GetSentPackets();
                    unsigned int acked_packets = connection.GetReliabilitySystem(i).GetAckedPackets();
                    unsigned int lost_packets = connection.GetReliabilitySystem(i).GetLostPackets();
                    
                    float sent_bandwidth = connection.GetReliabilitySystem(i).GetSentBandwidth();
                    float  acked_bandwidth = connection.GetReliabilitySystem(i).GetAckedBandwidth();
                    printf( "For player %d , Port %d : rtt %.1fms, sent %d, acked %d, lost %d (%.1f%%), sent bandwidth = %.1fkbps, acked bandwidth = %.1fkbps\n", i , address[i].GetPort(),
                           (rtt * 1000.0f), sent_packets, acked_packets, lost_packets,
                           sent_packets > 0.0f ? (float) lost_packets / (float) sent_packets * 100.0f : 0.0f,
                           sent_bandwidth, acked_bandwidth );
                }
            }
            statsAccumulator -= 0.25f;
        }*/
        
        usleep( (int) ( DeltaTime * 1000000.0f ) );
        
        }
    }
    else{
        maindisplay((void*)1);
    }
}
