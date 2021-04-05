#include "bootstraper.h"

//globals
char *flags[5];

sockaddr_in client;
socklen_t clientSize = sizeof(client);

int clientSocket;

char host[NI_MAXHOST];
char service[NI_MAXSERV];


//functions
void connectionHandler(int listening){
    listen(listening, SOMAXCONN);

    cout << "Waiting for connection..." << endl;

    //Waiting for connection
    clientSocket = accept(listening, (sockaddr *)&client, &clientSize);

    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);

    if (getnameinfo((sockaddr *)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) != 0)
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);

    cout << host << " connected on port " << ntohs(client.sin_port) << endl;

    //close listener socket
    close(listening);
}

char **split(char *__string, const char *__delimiter, char *__array[]) {
    int index = 0;

    char *token = strtok(__string, __delimiter);

    while (token){
        __array[index] = token;
        index = index + 1;
        token = strtok(NULL, ",");
    }

    return __array;
}

int dirExists(const char* path){
    struct stat info;
    if(stat(path, &info) != 0) return 0;
    else if(info.st_mode & S_IFDIR) return 1;
    else return 0;
}


int init_bootstrap(){
    //TODO: add spdlog for better loggings and verbose!!

    //folder checker
    const char* path = "./settings/";
    if(!dirExists(path)){
        if(mkdir(path,0777) == -1){
            cerr << "ERR - " << strerror(errno);
            return EPERM;
        }
    }else cout << "Directory Already exists!" << endl;

    //Listener Creation
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1) {
        cerr << "ERR - Can't create socket"; 
        return ENOTCONN;
    }

    cout << "Listener created." << endl;

    //Bind ip to created socket
    sockaddr_in hint;
    bool isOpen = false;
    hint.sin_family = AF_INET;
    cout << "checking ports 1024 ... 16384" << endl;
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    //checking for open ports
    for(int port = 49152;port <= 65535; ++port){
        hint.sin_port = htons(port);
        if (bind(listening, (sockaddr*)&hint, sizeof(hint)) != -1){
            cout << port << " is available to connect!" << endl;
            cout << "Writing port number into settings/connection.json!" << endl;

            //writing connection.json file        
            ofstream file;
            file.open("./settings/connection.json", ofstream::out);
            file << "{\n\t\"port\" : " << port <<",\n\t\"ip\" : \"0.0.0.0\"\n}";
            file.close();
            isOpen = true;
            break;
        }
    }
    
    if(!isOpen){
        cerr << "ERR - Can't bind socket to IP/Port";
        return EADDRNOTAVAIL;
    }

    cout << "Socket binded to IP." << endl;

    //First connection handler
    connectionHandler(listening);

    //Accepeting front-end flags
    char buff[MAX_BUFF];
    char response[MAX_BUFF];

    while(true){
        memset(buff, 0, MAX_BUFF);
        memset(response, 0, MAX_BUFF);

        //waiting for flags
        int bytesReceived = recv(clientSocket, buff, MAX_BUFF, 0);
        if(bytesReceived == -1){
            cerr << "ERR - cannot recive flags. Quitting..." << endl;
            break;
        }

        if(bytesReceived == 0 || buff == "EXIT"){
            cout << "Client disconnected..." << endl;
            break;
        }

        split(buff, ",", flags);

        double _first = atof(flags[0]);
        double _second = atof(flags[1]);
        char *_op = flags[2];

        double result = math(_first, _second, _op);

        //Echo flag resualt to program!!

        sprintf(response, "%f", result);
        send(clientSocket, response, 5, 0);
        cout << "Sent " << response << " to client " << result << endl;
        // send(clientSocket, buff, bytesReceived + 1, 0);
    }

    close(clientSocket);
    return 0;
}