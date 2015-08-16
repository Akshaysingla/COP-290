#include "Maze.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

void initialize ( grid** maze, int num ) {
	for ( int i = 0; i < num; i++ ) {
		for ( int j = 0; j < num; j++ ) {
			for ( int k = 0; k < 4; k++ )
				maze[i][j].edge[k] = 1;
			maze[i][j].visited = 0;
		}
	}
}

void prims ( grid** maze, int num ) {
	srand ( time(NULL));
	vector<coord> map;
	int i, j;
	i = rand()%(num-2)+1;
	j = rand()%(num-2)+1;
	cout << i << " " << j << endl;
	maze[i][j].visited = 2;
	map.push_back ( coord ( i+1, j ) );
	map.push_back ( coord ( i-1, j ) );
	map.push_back ( coord ( i, j+1 ) );
	map.push_back ( coord ( i, j-1 ) );
	for ( i = 0; i < (num*num); i++ ) {
		//cout << "A:" << map.size() << endl;
		if ( map.size() == 0 ) break;
		int select = rand()%(map.size());
		int X = map[select].x;
		int Y = map[select].y;
		map.erase ( map.begin() + select );
		maze[X][Y].visited = 2;
		vector<coord> neighbour;
		if ( X+1 < num ) {
			if ( maze[X+1][Y].visited == 0 ) {
				map.push_back ( coord ( X+1, Y ) );
				maze[X+1][Y].visited = 1;
			}
			else if ( maze[X+1][Y].visited == 2 )
				neighbour.push_back ( coord ( X+1, Y, 1 ) );
		}
		if ( X-1 >= 0 ) {
			if ( maze[X-1][Y].visited == 0 ) {
				map.push_back ( coord ( X-1, Y ) );
				maze[X-1][Y].visited = 1;
			}
			else if ( maze[X-1][Y].visited == 2 )
				neighbour.push_back ( coord ( X-1, Y, 3 ) );
		}
		if ( Y+1 < num ) {
			if ( maze[X][Y+1].visited == 0 ) {
				map.push_back ( coord ( X, Y+1 ) );
				maze[X][Y+1].visited = 1;
			}
			else if ( maze[X][Y+1].visited == 2 )
				neighbour.push_back ( coord ( X, Y+1, 2 ) );
		}
		if ( Y-1 >= 0 ) {
			if ( maze[X][Y-1].visited == 0 ) {
				map.push_back ( coord ( X, Y-1 ) );
				maze[X][Y-1].visited = 1;
			}
			else if ( maze[X][Y-1].visited == 2 )
				neighbour.push_back ( coord ( X, Y-1, 0 ) );
		}
		//cout << "B:" << neighbour.size() << endl;
		select = rand()%(neighbour.size());
		maze[X][Y].edge[neighbour[select].edgeRel] = 0;
		maze[neighbour[select].x][neighbour[select].y].edge[(neighbour[select].edgeRel+2)%4] = 0;
	}
	
	for(int i = num/2 -2 ; i <= num/2 +1 ; i++ ) {
		for(int j = num/2 -2 ; j <= num/2 +1 ; j++ ) {
			for(int k = 0 ; k < 4 ; k ++ ) {
				if ( j == num/2 - 2 && k == 0 )
					continue;
				if ( j == num/2 + 1 && k == 2 )
					continue;
				if ( i == num/2 - 2 && k == 3 )
					continue;
				if ( i == num/2 + 1 && k == 1 )
					continue;
				maze[i][j].edge[k] = 0;
			}
		}
	}

	for ( int i = 0; i < num; i++ ) {
		for ( int j = 0; j < num; j++ ) {
			if ( maze[i][j].edge[0]==1 && maze[i][j].edge[1]==1 && maze[i][j].edge[2]==1 && maze[i][j].edge[3]==1 ) {
				//cout << "griddddddd" << endl;
				if ( i != 0 && i != num ) maze[i][j].edge[3] = 0;
				else {
					if ( j != 0 ) maze[i][j].edge[0] = 0;
					else maze[i][j].edge[2] = 0;
				}
			}
		}
	}
}