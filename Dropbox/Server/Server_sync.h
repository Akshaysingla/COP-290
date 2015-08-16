#ifndef SERVER_SYNC_H
#define SERVER_SYNC_H

#include "Server_dropbox.h"
#include "SQL/include/sqlite3.h"

class ServerSync {
	Server_dropbox *Dropbox;
	SSL *ssl;
public:
	ServerSync(Server_dropbox *, SSL* );
	void SyncAll (char*);
	void AllFilesFromServer (char*);
	void AllFilesFromClient (char*);
	void SyncClientServer (char*);


};

#endif // SERVER_SYNC_H