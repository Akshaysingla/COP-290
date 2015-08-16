# Makefile for executable adjust

# *****************************************************
# Parameters to control Makefile operation

CC = g++ -DDEBUG_CONNECT -DDEBUG_SYNC -DDEBUG_SQL -g
TEXFILE = latexsrc/Assignment1
CFLAGS = -std=c++11 -w -L/../../home/dual/cs5130168/Desktop/Dropbox/Server/SQL/lib -lsqlite3 -Wall -L/usr/lib -lssl -lcrypto

# ****************************************************
# Entries to bring the executable up to date

server: Server_dropbox.o Server_SSL.o Server_register.o Server_login.o Server_sync.o Server_share.o main.o
	$(CC) -o server Server_dropbox.o Server_SSL.o Server_register.o Server_login.o Server_sync.o Server_share.o main.o $(CFLAGS)

main.o: main.cpp Server_dropbox.h
	$(CC) $(CFLAGS) -c main.cpp -o main.o

Server_dropbox.o:  Server_dropbox.cpp Server_SSL.h Server_register.h Server_login.h Server_sync.h Server_share.h
	$(CC) $(CFLAGS) -c Server_dropbox.cpp -o Server_dropbox.o

Server_SSL.o:  Server_SSL.cpp
	$(CC) $(CFLAGS) -c Server_SSL.cpp -o Server_SSL.o

Server_register.o:  Server_register.cpp
	$(CC) $(CFLAGS) -c Server_register.cpp -o Server_register.o

Server_login.o:  Server_login.cpp
	$(CC) $(CFLAGS) -c Server_login.cpp -o Server_login.o

Server_sync.o:  Server_sync.cpp Server_dropbox.h
	$(CC) $(CFLAGS) -c Server_sync.cpp -o Server_sync.o

Server_share.o:  Server_share.cpp
	$(CC) $(CFLAGS) -c Server_share.cpp -o Server_share.o

clean:
	rm server
	rm *.o

DEBUG_CONNECT: CC += -DDEBUG_CONNECT -g

DEBUG_SYNC: CC += -DDEBUG_SYNC -g

DEBUG_SQL: CC += -DDEBUG_SQL -g

DEBUG_ALL: CC += -DDEBUG_CONNECT -DDEBUG_SYNC -DDEBUG_SQL -g