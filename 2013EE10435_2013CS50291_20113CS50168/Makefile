# Makefile for executable adjust

# *****************************************************
# Parameters to control Makefile operation

CC = g++
TEXFILE = latexsrc/Assignment1
CFLAGS = -lGL -lGLU -lglut -lglui -pthread -lSDL -lSDL_image

# ****************************************************
# Entries to bring the executable up to date

all: NUM_THREADS=5
all: Run

execute: Run

DEBUG_ALL: CC += -DDEBUG_WALL -DDEBUG_BALL -DITERATION -DPQUEUE_TOP -g
DEBUG_ALL: clean executables/ScreenSaver Run clean2

DEBUG_BALL_COL: CC += -DDEBUG_BALL -g
DEBUG_BALL_COL: clean executables/ScreenSaver Run clean2

DEBUG_WALL_COL: CC += -DDEBUG_WALL -g
DEBUG_WALL_COL: clean executables/ScreenSaver Run clean2

DEBUG_ITER: CC += -DITERATION -g
DEBUG_ITER: clean executables/ScreenSaver Run clean2

DEBUG_PQUEUE: CC += -DPQUEUE_TOP -g
DEBUG_PQUEUE: clean executables/ScreenSaver Run clean2

Run: executables/ScreenSaver
	./executables/ScreenSaver $(NUM_THREADS)

executables/ScreenSaver: obj/Update.o obj/Menu.o obj/Collision.o obj/Display.o obj/PriorityQueueArray.o obj/Main.o
	$(CC) -o executables/ScreenSaver obj/Update.o obj/Menu.o obj/Collision.o obj/Display.o obj/PriorityQueueArray.o obj/Main.o $(CFLAGS)

obj/Main.o: src/Main.cpp src/Structures.h src/PriorityQueueArray.h src/Variables.h src/Display.h src/Menu.h src/Update.h
	$(CC) $(CFLAGS) -c src/Main.cpp -o obj/Main.o

obj/Menu.o: src/Menu.cpp src/Structures.h src/PriorityQueueArray.h src/Variables.h src/Display.h
	$(CC) $(CFLAGS) -c src/Menu.cpp -o obj/Menu.o

obj/Collision.o: src/Collision.cpp src/Structures.h src/PriorityQueueArray.h src/Variables.h src/Update.h
	$(CC) $(CFLAGS) -c src/Collision.cpp -o obj/Collision.o 

obj/Display.o: src/Display.cpp src/Structures.h src/PriorityQueueArray.h src/Variables.h src/Menu.h
	$(CC) $(CFLAGS) -c src/Display.cpp -o obj/Display.o

obj/Update.o: src/Update.cpp src/Structures.h src/PriorityQueueArray.h src/Variables.h src/Collision.h
	$(CC) $(CFLAGS) -c src/Update.cpp -o obj/Update.o

obj/PriorityQueueArray.o: src/PriorityQueueArray.cpp src/Structures.h
	$(CC) $(CFLAGS) -c src/PriorityQueueArray.cpp -o obj/PriorityQueueArray.o

doc:
	latexmk -pdf -quiet $(TEXFILE)
	rm -f *.aux *.fdb_latexmk *.fls *.log

clean: 
	rm executables/ScreenSaver
	rm obj/*.o

clean2: 
	rm executables/ScreenSaver
	rm obj/*.o