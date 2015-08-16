#ifndef UPDATE_H_
#define UPDATE_H_

//#include "Structures.h"
struct b;

float random_range(int x , int y);
void Initialize(long id, b &ball);
void *update ( void *tid );
void update_pqueue (long id, int i, b ball1, b ball2 );

#endif
