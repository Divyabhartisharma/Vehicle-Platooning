// Server side C/C++ program to demonstrate Socket
// programming
#include <Ws2tcpip.h>
#include <iostream>
#include <pthread.h>


#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080
#define NUMTHREADS 100

using namespace std;

void *socketThread(void *arg)
{
    SOCKET newSocket = *(SOCKET *)arg;
    char buf[4096];
    //Wait for client to send data
    while(true){
        ZeroMemory(buf, 4096);
        int byteReceived = recv(newSocket, buf, 4096, 0);
        if(byteReceived == SOCKET_ERROR){
            cerr<<"Error in recv(). Quitting" <<endl;
            break;
        }

        if (byteReceived == 0)
        {
            cout << "Client disconnected " << endl;
            break;
        }
        //Send message back to client
        send(newSocket, buf, byteReceived + 1, 0);
        cout<<string(buf, 0, byteReceived)<<endl;
    }

    //Close the socket
    closesocket(newSocket);
    return 0 ;

}

int main(int argc, char **argv){
    pthread_t threads[NUMTHREADS];

    //Init return(1);winsock
    WSADATA wsaData;
    WORD ver = MAKEWORD(2, 2);

    int wsOk = WSAStartup(ver, &wsaData);
    if (wsOk != 0)
    {
        cerr << "Can't Initialize winsock! Quitting" << endl;
        
    }

    //Create a socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if(listening == INVALID_SOCKET){
        cerr<<"Can't create a socket! Quitting" <<endl;
        return(1);
    }

    //Bind the ip address and port to a socket
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton ....

    bind(listening, (sockaddr*)&serverAddr, sizeof(serverAddr));

    //Tell Winsock the socket is for listening
    listen(listening, SOMAXCONN);

    pthread_t tid[60];
    int i = 0;

    while(1){
        sockaddr_in client;
        int clientSize = sizeof(client);
        SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
        if(clientSocket == INVALID_SOCKET){
            cerr<<" invalid socket! Quitting" << endl;
            return(1);
        }
        char host[NI_MAXHOST]; // Client's remote name
        char service[NI_MAXHOST]; // Service (i.e. port) the client is connect on

        ZeroMemory(host, NI_MAXHOST);
        ZeroMemory(service, NI_MAXHOST);

        if(getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0){
            cout<<host<<" connected on port "<<service<<endl;
        }else{
            inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
            cout<<host<<" connected on port "<<ntohs(client.sin_port)<<endl;
        }
        if (pthread_create(&threads[i++], NULL, socketThread, (void *)&clientSocket) != 0){
            cerr <<"Failed to create thread! Err #" << WSAGetLastError() << endl;
            return(1);
        }else{
            cout<<"Thread created!" <<endl;
        }
        
    }
    //Close listening socket
    closesocket(listening);

    //Cleanup winsock
    WSACleanup();
    return(0);

}