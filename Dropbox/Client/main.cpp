#include <iostream>
#include "Client_dropbox.h"

using namespace std;

int main ( int argc, char *argv[] ) {
    int option;
    Client_dropbox create_client(argv[1]);
    create_client.CreateSocketToServer();
    char temp[256];
    do {
        cout << "1. Upload File to Server\n";
        cout << "2. Download File from Server\n";
        cout << "3. Logout\n\n";
        cout << "Enter Choice -> ";
        cin >> option;
        switch (option) {
            case 1 :
                cout << "\n\nEnter Filename with extension -> ";
                cin >> temp;
                create_client.UploadFile (temp);
                break;
            case 2 : 
                cout << "\n\nEnter Filename with extension -> ";
                cin >> temp;
                create_client.DownloadFile (temp);
                break;
            case 3 :
                create_client.deleteSocket();
        }
    } while ( option != 3 );
    return 0;
}