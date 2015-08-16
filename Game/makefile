# Makefile for executable adjust

# *****************************************************
# Parameters to control Makefile operation

CC = g++
CFLAGS = -w -lGL -lGLU -lglut -lglui -lSDL -lSDL_image -lpthread

# ****************************************************
# Entries to bring the executable up to date

labyrinth: obj/Main.o obj/Maze.o
	$(CC) -o executable/labyrinth obj/Main.o obj/Maze.o $(CFLAGS)

obj/Main.o: src/Main.cpp src/Net.h src/Structures.h src/Maze.h src/Variables.h
	$(CC) $(CFLAGS) -c src/Main.cpp -o obj/Main.o

obj/Maze.o: src/Maze.cpp src/Net.h src/Structures.h
	$(CC) $(CFLAGS) -c src/Maze.cpp -o obj/Maze.o

clean:
	rm executable/labyrinth
	rm obj/*.o

single:
	make labyrinth
	executable/labyrinth $(AI)

multi_host:
	make labyrinth
	executable/labyrinth $(PORT) $(PLAY) $(AI) 1

multi_join:
	make labyrinth
	executable/labyrinth $(IP) $(HOST) $(PORT)