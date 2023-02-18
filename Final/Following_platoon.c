   #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>

#define SERVER_PORT 8000
#define SERVER_IP "127.0.0.1" 
//#define BUFFER_SIZE 1024
#define MSG_CLIENT_TO_SERVER "CLIENT_MSG"
#define MSG_SERVER_TO_CLIENT "SERVER_MSG"




int main(){
   struct  sockaddr_in serverAddr;
   int valread,i;
   //char buffer[BUFFER_SIZE] = {0};
    //char buffer[1024];
     char *message = "Hi";
    char buffer[1024] = {0};
    //create new sock for Following_client 
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Could not create socket");
        return 1;
    }
    printf("Following vehicle in range with leading vehicle...\n");
    

    memset(&serverAddr, '\0',sizeof(serverAddr));
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    int Connect_leadtruck=connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if ( Connect_leadtruck< 0) {
        perror("no data passing lost communication...\n");
        return 1;
    }
    else{
        printf("Communication occured with leading vehicle....\n");
    }
    //once it got connected to Lead_Server we check the connection is established continuously

    //cretae message buffer 
    
while (1)
{
        printf("Following_Vehicle:..\t");
        scanf("%s",&buffer[0]);
        send(sock,buffer,strlen(buffer),0);
    
    //also check here weather server is accepting the client check with below code 
        if(strcmp(buffer, ":exit")==0)
    {
        close(sock);
        printf("Disconneted to server..\n.");
        exit;
    }

    printf("Server accepted the request..\n");
    // Send "Hi" message to Lead_Server
    
    send(sock, message, strlen(message), 0);
    printf("Hello message sent\n");

    // Receive "Connected" message from Lead_Server
    valread = read(sock, buffer, 1024);
    printf("Lead_Server response: %s\n", buffer);
    memset(buffer, 0, sizeof(buffer)); // clear buffer

    // Send speed request to Lead_Server
    strcpy(buffer, "What is the speed?");
    send(sock, buffer, strlen(buffer), 0);
    printf("Speed request sent to Lead Vehicle:\n");
    memset(buffer, 0, sizeof(buffer)); // clear buffer

    // Receive speed from Lead_Server

    valread = read(sock, buffer, 1024);
    printf("Lead_Server response: %s\n", buffer);
    memset(buffer, 0, sizeof(buffer)); // clear buffer

    // Send distance request to Lead_Server
    strcpy(buffer, "What is the distance?");
    send(sock, buffer, strlen(buffer), 0);
    printf("Distance request sent to Lead Vehicle:\n");
    memset(buffer, 0, sizeof(buffer)); // clear buffer



    // Receive distance from Lead_Server
    valread = read(sock, buffer, 1024);
    printf("Lead_Server response: %s\n", buffer);
    memset(buffer, 0, sizeof(buffer)); // clear buffer

    // Send intrusion message to Lead_Server
    strcpy(buffer, "intrusion");
    send(sock, buffer, strlen(buffer), 0);
    printf("Following_client: %s\n", buffer);
    memset(buffer, 0, sizeof(buffer)); // clear buffer

    // Receive instruction from Lead_Server
    valread = read(sock, buffer, 1024);
    printf("Lead_Server send response : %s\n", buffer);
    memset(buffer, 0, sizeof(buffer)); // clear buffer

    // Acknowledge instruction from Lead_Server
    strcpy(buffer ,"ok");
    send(sock, buffer, strlen(buffer), 0);
    printf("Following_client: %s\n", buffer);
    memset(buffer, 0, sizeof(buffer)); // clear buffer

   
}  
    return 0;
    
}
   
  
 








