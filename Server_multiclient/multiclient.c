#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>

//define a macro values of the port

#define PORT 5555

//initiate client also connection by which the server is connecting

int main(){
    int clientSocket,connection;
    struct  sockaddr_in serverAddr;
    char buffer[1024];

//Here client socket is getting protocol from TCP and INET libarry

  
    clientSocket=socket(AF_INET,SOCK_STREAM,0);

// comparision to check weather connection is successful or not 

    if(clientSocket<0){
        printf("Error in connection of client1....\n");
        exit;
    }
    printf("Client1 connected...\n");

// below is mentioned connection of ServerAddr with post ,memory size etc..
    memset(&serverAddr, '\0',sizeof(serverAddr));
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(PORT);
    serverAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    connection = connect(clientSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    if(connection<0){
        printf("not able to check server connection \n");
        exit;
    }
    printf("Connection established with the server...\n");

//once it got connected to server we check the connection is established continuously
    while (1)
    {
        printf("Client:..\t");
        scanf("%s",&buffer[0]);
        send(clientSocket,buffer,strlen(buffer),0);
    
    //also check here weather server is accepting the client check with below code 
    if(strcmp(buffer, ":exit")==0)
    {
        printf("Disconneted to server..\n.");
        exit;
    }
    printf("Server accepted the request..\n");
//After receiving the message
    if(recv(clientSocket,buffer,1024,0)<0){
        printf("Again isssue in receiving...dicontinued\n");
    }
    else{
        printf("server send back to client: \t%s\n",buffer);
        }
    }
    return 0;


}