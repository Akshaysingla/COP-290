#include <stdio.h>
#include <stdlib.h>
#include "SQL/include/sqlite3.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int main(int argc, char* argv[])
{
   sqlite3 *db;
   char *zErrMsg = 0;
   int  rc;
   char *sql;

   /* Open database */
   rc = sqlite3_open("MyDropbox.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stdout, "Opened database successfully\n");
   }

   /* Create SQL statement */
   sql = "CREATE TABLE DROPBOX("  \
         "USERNAME          TEXT    NOT NULL," \
         "FILENAME            TEXT     NOT NULL," \
         "FILEPATH        TEXT  NOT NULL," \
         "FILEID          INT NOT NULL," \
         "EXTENSION       TEXT  NOT NULL," \
         "DELETED        INT  NOT NULL," \
         "LAST_EDITED     TEXT NOT NULL," \
         "SHARED_FROM      TEXT,"\
         "FILESIZE			INT NOT NULL,"\
         "HASH			TEXT NOT NULL,"\
         "OWNERS		INT NOT NULL );";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Table created successfully\n");
   }
   /* Create SQL statement */
   sql = "CREATE TABLE USERS("  \
         "USERNAME          TEXT    NOT NULL," \
       	 "PASSWORD			TEXT	NOT NULL, " \
       	 "FIRSTNAME		TEXT	NOT NULL, " \
         "LASTNAME	TEXT	NOT NULL,"\
         " LASTSYNC  TEXT )";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Table created successfully\n");
   }
   sqlite3_close(db);
   return 0;
}
