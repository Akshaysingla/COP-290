#ifndef STRUCTURES_H_
#define STRUCTURES_H_

struct grid {
	bool edge[4];		// 0-top, 1-right, 2-left, 3-bottom 
	int visited;		// 0-unvisited, 1-neighbour, 2-visited
};

class coord {
public:
	coord ( int a, int b ) {
		x = a; y = b;
	}
	coord ( int a, int b, int c ) {
		x = a; y = b; edgeRel = c;
	}
	int x;
	int y;
	int edgeRel;
};
struct powerUp {
    bool exist;
    int gridx, gridy;
    float timestamp;
    int type;       //0 - Health , 1 - Mana , 2 - Shield
};
enum Mode
    {
        Player,
        Host
    };
enum GameType{
    Single,
    Multi
};
class node {
public:
    node ( int a, int b ) {
        x = a; y = b;
    }
    int x;
    int y;
};
struct pos {
    int my_id;
    bool ai;
    float x;
    float y;
    int gridx;
    int gridy;
};
struct spell {
	int type;
    float x;
    float y;
    int gridx;
    int gridy;
    float lx;
    float ly;
    int exist;
    int my_shooter;
};
struct playerstats{
    int health; 
    int mana;
    float shield;
    int hit;
};
#endif
