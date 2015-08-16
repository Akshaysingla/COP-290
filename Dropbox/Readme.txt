MyDropBox Version 1.0  03/03/2015

GENERAL INFORMATION
-------------------------------

-	MyDropBox is a cloud based file sharing and file storage application which allows
	the user to store and share files on a server.
-	The information is exchanged P2P over a secured SSL connection.
-	MyDropBox allows offline data editing and automatic cloud sync.


INSTALLATION INSTRUCTIONS
-------------------------------

-	The application has been built for Linux and Mac systems containing the
	following c++ libraries : Qt Creator, C++11, OpenSSL library and the standard 
	libraries. 
-	Open the "MyDropBox" folder containing all the files 
	in the terminal and run make to run the makefile to compile program. 
	Make the server using "./Server {Port No}".
	Connect the client to the server using "./Client {Server Name} {Port No}"

GUI Option
-------------------------------
- Sync All: Syncs the MyDropBox folder with the server. Leading to the information on your
  	    and MyDropBox folder being identical.

- Open DropBox: Opens your MyDropBox folder.

- Logout: Logs you out of your account and closes the server connection.

- Browse files: Opens a directory tree of your computer for you to select multiple files
	        that you might wish to transfer.

- Upload files: Uploads the selected files to the server.

- Share file: Shares a file with another user of your choice. You can either transfer it
	      or collaborate with the other user. Kindly select only one file.

- Change Privilege: Alters the shared privilege of an already shared file.

- Unshare: Unshare an already shared file.

- Delete file: Deletes a file from your MyDropBox and server.

- Download file: Downloads a file that has been shared with you.

DEBUGGING MODE
-------------------------------

-	There is a debugging mode to remove any bugs which might be present in the 
	software. The following Debugging modes/symbols are present:

-	a)	DEBUG_SYNC: Displays the server messages to ensure proper sync progress.
		Provides the status of each file before and after transfer.
-	b)	DEBUG_SQL: Displays the SQL Queries to ensure their proper construction
		during the program.
-	c)	DEBUG_CONNECT: Displays the status of the connection and SSL related
		messages.
-	d)	DEBUG_ALL: Displays all of the above together.

-	These can be run using make DEBUG_<symbol>

CONTACT INFORMATION
-------------------------------

For updates and other details contact the following:
-	Akshay Singla: ee1130435@iitd.ac.in
-	Nikhil Chaturvedi: cs5130291@iitd.ac.in
-	Rishabh Sanghi: cs5130168@iitd.ac.in


PRECAUTIONS
-------------------------------
Do not delete the MyDropBox_{Username} folder. 
Do not delete the configuration file which is hidden in the Client folder.

References
--------------------------------
-	qt-project.org
-	http://www.linuxhowtos.org/C_C++/socket.html
-	http://simplestcodings.blogspot.in/2010/08/secure-server-client-using-openssl-in-c.html
-	http://stackoverflow.com
-	For icons:
	https://images.google.com/?gws_rd=ssl


