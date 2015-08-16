#ifndef VARIABLES_H_
#define VARIABLES_H_

#include <GL/glui.h>
#include <GL/glut.h>
#include <vector>
#include "Structures.h"
#include "Net.h"

using namespace std;
using namespace net;


extern bool mouseWarp;
extern const int ProtocolId;
extern const float DeltaTime;
extern const float SendRate;
extern const float TimeOut;
extern const int PacketSize;
extern int SpellSelected;

#define HEALTHMAX 10
#define MANAMAX 100
#define SHIELD 10
#define PLAYERMAX 6

extern int main_window;
extern int display_which ;
extern int time_left;
extern int ToalBullets;
extern int c;
extern int players;
extern int AI_number;
extern char** v;
extern int HealthAngle;
extern const int N;
extern float powerUpCubeLength;
extern float playerRadius;


extern int SpellID;
extern GLfloat SpellColour[3][3];
extern GLfloat PlayerColour[7][3];

extern int playerFreeze[PLAYERMAX];

extern Mode mode;
extern GLUI *sub_w;


extern bool AI_Thread[PLAYERMAX];

extern bool display_player[PLAYERMAX];

extern GameType gamestyle;

extern const int numpowerUp;
extern powerUp powerUps[25];
extern int powerUpangle;

extern bool keyup;
extern float view_rotate[16];
extern float view_rotate_init[16];

extern float edgeLength;
extern float wallThickness;
extern float mazeHeight;
extern grid** maze;
extern float _angle;
extern GLuint texture[8];
extern int torangle;

extern int mynumber;

extern int networkplayers;

extern float bulletRadius;

extern float lx;
extern float ly;
extern float prevlx, prevly;
extern float prevx, prevy;
extern int prevgridx, prevgridy;
extern float angle;
extern float deltaAngle;
extern float xOrigin;

extern bool visit[PLAYERMAX][22][22];

extern int window_id;

extern GLUI *glui_st;

extern pos otherPlayers[PLAYERMAX];

extern pos rec_pos[PLAYERMAX];
extern float lx2;
extern float ly2;

extern vector<spell> RecSpellCollect;
extern spell tempspellRec;

extern vector<spell*> spell_on_screen;

extern vector<spell> PendingBulletsSend;


extern playerstats mystat[PLAYERMAX];


extern int orbs[4];

extern int count_down;

extern GLUquadricObj *obj1;
extern GLUquadricObj *obj2;
extern GLUquadricObj *obj3;


#endif