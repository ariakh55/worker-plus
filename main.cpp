#include "lib/header.h"
#include "lib/flags.h"

int main(){
    //TODO: add spdlog for better loggings and verbose!!

    //Listener Creation
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1) {
        cerr << "ERR - Can't create socket"; 
        return ENOTCONN;
    }

    cout << "Listener created." << endl;

    //Bind ip to created socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
    
    if(bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1){
        cerr << "ERR - Can't bind socket to IP/Port";
        return EADDRNOTAVAIL;
    }

    cout << "Socket binded to IP." << endl;

    listen(listening, SOMAXCONN);

    cout << "Waiting for connection..." << endl;

    //Waiting for connection
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);

    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
    
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);

    if(getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) != 0)
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);

    cout << host << " connected on port " << ntohs(client.sin_port) << endl;

    //close listener socket
    close(listening);

    //Accepeting front-end flags
    char buff[MAX_BUFF];
    
    while(true){
        memset(buff, 0, MAX_BUFF);

        //waiting for flags
        int bytesReceived = recv(clientSocket, buff, MAX_BUFF, 0);
        if(bytesReceived == -1){
            cerr << "ERR - cannot recive flags. Quitting..." << endl;
            break;
        }

        if(bytesReceived == 0){
            cout << "Client disconnected..." << endl;
            break;
        }

        cout << string(buff, 0, bytesReceived) << endl;

        //Echo flag resualt to program!! -- TODO
        send(clientSocket, buff, bytesReceived + 1, 0);
    }

    close(clientSocket);
    return 0;
}