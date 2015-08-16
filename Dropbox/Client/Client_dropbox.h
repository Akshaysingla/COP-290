class Client_dropbox{
    public :
    Client_dropbox(char *HostName, int);
    int CreateSocketToServer();
    bool socket_to_server();
    void deleteSocket();
    
private:
    char *HostName;
    int port_no;
    int client_socket;
    bool sockettoserver;
};

