#ifndef COLLISION_H_
#define COLLISION_H_

//#include "Structures.h"
struct b;

void collision_ball ( long id, int from, b &ball1, b &ball2);
void collision_wall ( int id, b &ball, char wall, int dir );

#endif
