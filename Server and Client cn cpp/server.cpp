
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main()
{
    /* ---------- INITIALIZING VARIABLES ---------- */ 

    int client, server; // File descriptors store the values returned by the socket system call and the accept system call.
    
    int portNum = 1500;  // portNum is for storing port number on which the accepts connections

    bool isExit = false; // bool variable which will be used to end the loop
    
    int bufsize = 1024; // The server reads characters from the socket connection into a  variable (buffer).

    char buffer[bufsize];

    /* struct sockaddr_in
        {
          short   sin_family;
          u_short sin_port;
          struct  in_addr sin_addr;
          char    sin_zero[8];
        };
    */

    struct sockaddr_in server_addr;  // A sockaddr_in is a structure containing an internet address. 
   
    socklen_t size; // intr type of width of at least 32 bits

    /* ---------- ESTABLISHING SOCKET CONNECTION ----------*/
    /* --------------- socket() function ------------------*/

    client = socket(AF_INET, SOCK_STREAM, 0); // create socket 

    if (client < 0) 
    {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }



    cout << "\n=> Socket server has been created..." << endl;


    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);

    /* ---------- BINDING THE SOCKET ---------- */
    /* ---------------- bind() ---------------- */


    if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) 
    {
        cout << "=> Error binding connection, the socket has already been established..." << endl;
        return -1;
    }

    size = sizeof(server_addr);
    cout << "=> Looking for clients..." << endl;

    /* ------------- LISTENING CALL ------------- */
    /* ---------------- listen() ---------------- */

    listen(client, 1);

   

    int clientCount = 1;
    server = accept(client,(struct sockaddr *)&server_addr,&size);

    // first check if it is valid or not
    if (server < 0) 
        cout << "=> Error on accepting..." << endl;

    while (server > 0) 
    {
        strcpy(buffer, "=> Server connected...\n");
        send(server, buffer, bufsize, 0);
        cout << "=> Connected with the client #" << clientCount << ", you are good to go..." << endl;
        cout << "\n=> Enter # to end the connection\n" << endl;

        cout << "Client: ";
        do {
            recv(server, buffer, bufsize, 0);
            cout << buffer << " ";
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != '*');

        do {
            cout << "\nServer: ";
            do {
                cin >> buffer;
                send(server, buffer, bufsize, 0);
                if (*buffer == '#') {
                    send(server, buffer, bufsize, 0);
                    *buffer = '*';
                    isExit = true;
                }
            } while (*buffer != '*');

            cout << "Client: ";
            do {
                recv(server, buffer, bufsize, 0);
                cout << buffer << " ";
                if (*buffer == '#') {
                    *buffer == '*';
                    isExit = true;
                }
            } while (*buffer != '*');
        } while (!isExit);

       
        cout << "\n\n=> Connection terminated with IP " << inet_ntoa(server_addr.sin_addr);
        close(server);
        cout << "\nGoodbye..." << endl;
        isExit = false;
        exit(1);
    }

    close(client);
    return 0;
}