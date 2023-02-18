//#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<sys/types.h>
//#include "Leadingvehicle.c"
//#include "Followingvehicle.c"
#define SERVER_PORT 8000
#define SERVER_IP "127.0.0.1" 




//Defined all the states here 



int main()
{
    int server_sock, client_sock, c,valread;
    struct sockaddr_in server_addr, client_addr;
    char *response="Ackknowlede";
    char buffer[1024] = {0};
    char *hello = "Connected";
    int speed = 60;
    int distance = 10;
    int new_speed=40;
    int new_distance=20;
    
    pid_t childpid;
    // Create socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        printf("Could not create socket....\n");
        return 1;
    }
    printf("Check_from_leading_vehicle...\n");

    // Bind socket to port for leading vehicle
    memset(&server_addr,'\0',sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    server_addr.sin_port = htons(SERVER_PORT);

    

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Bind failed to Leading vehicle....\n");
        return 1;
    }
    else{
        printf("Bind Successful with Lead vehicle.....%d\n",8888);
    }

    // now Listen to following vehicle 
    if(listen(server_sock,4)==0){
        printf("Listening to following vehicles...\n");
    }
    else{
        printf("communication lost with FV");
    }

    // Accept incoming connection

    c = sizeof(struct sockaddr_in);
    socklen_t addr_size;
    while (1){
     client_sock = accept(server_sock,(struct sockaddr*)&client_addr,&addr_size);
    //client_sock = accept(server_sock, (struct sockaddr *)&client_addr, (socklen_t*)&c);
    if (client_sock < 0) {
        perror("Accept failed");
        return 1;
    }

    // valread = read(client_sock, buffer, 1024);
    //   if (valread < 0) {
    //     perror("Read failed");
    //     exit(EXIT_FAILURE);
    // }

    //printf("Received message from Following_client: %s\n", msg_buffer);
    printf("Following_vehicle_Connection accepted %s:%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
    if((childpid = fork())==0){
       close(server_sock); 
    
            recv(client_sock,buffer,1024,0);
            if(strcmp(buffer,":exit")==0){
                printf("disconnected from %s:%d\n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
                break;
            }
            else{
                printf("Following_client: %s\n",buffer);
                send(client_sock,buffer,strlen(buffer),0);
                bzero(buffer,sizeof(buffer));
                
            }
        
  // Send "Connected" message to Following_client
     while(1){
    
    printf("Hello from Lead_platoon\n");
    send(client_sock, hello, strlen(hello), 0);
    printf("Hello from Following_platoon\n");
    memset(buffer, 0, sizeof(buffer)); // clear buffer

    // Receive speed request from Following_client
    read(client_sock, buffer, 1024);
    printf("Following_Client: %s\n", buffer);
    memset(buffer, 0, sizeof(buffer)); // clear buffer

    // Send speed to Following_client
    sprintf(buffer, "%d", speed);
    send(client_sock, buffer, strlen(buffer), 0);
    printf("constant speed sent: %d\n", speed);
    memset(buffer, 0, sizeof(buffer)); // clear buffer

    // Receive distance request from Following_client
    read(client_sock, buffer, 1024);
    printf("Following_client: %s\n", buffer);
    memset(buffer, 0, sizeof(buffer)); // clear buffer

    // Send distance to Following_client
    sprintf(buffer, "%d", distance);
    send(client_sock, buffer, strlen(buffer), 0);
    printf("constant Distance between sent trucks : %d\n", distance);
    memset(buffer, 0, sizeof(buffer)); // clear buffer
    

    // Receive intrusion message from Following_client
    read(client_sock, buffer, 1024);
    printf("Following_client: %s\n", buffer);
    memset(buffer, 0, sizeof(buffer)); // clear buffer

    // Send distance to Following_client
    sprintf(buffer, "%d", new_speed);
    send(client_sock, buffer, strlen(buffer), 0);
    printf("new_speed maintain until object there:  %d\n", new_speed);
    memset(buffer, 0, sizeof(buffer)); // clear buffer

    read(client_sock, buffer, 1024);
    printf("Following_client: %s\n", buffer);
    memset(buffer, 0, sizeof(buffer)); // clear buffer


    // Send distance to Following_client
    sprintf(buffer, "%d", new_distance);
    send(client_sock, buffer, strlen(buffer), 0);
    printf("new_distance  maintain until object there : %d\n", new_distance);
    memset(buffer, 0, sizeof(buffer)); // clear buffer
    
     }
         
     
        }
    }
    
    close(client_sock);
    //close(server_sock);

    return 0;
    
}    
