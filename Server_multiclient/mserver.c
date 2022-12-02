#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>

#define port 5555

int main(){

// initialise socket 
    int sockid, connection;
    struct sockaddr_in serverAddr;

// now new socket with client 
    int newSocket;
    struct sockaddr_in newAddr;
    socklen_t addr_size;
    char buffer[1024];

//use fork function to handle multiple client
    pid_t childpid;
    sockid=socket(AF_INET,SOCK_STREAM,0);

// comparision to check weather connection is successful or not 
//Note: this part of server and client will be same becz 
//there is no different in setting the address with protocol family and port 

    if(sockid<0){
        printf("Error in connection of client1....\n");
        exit;
    }
    printf("Server Established...\n");
    memset(&serverAddr,'\0',sizeof(serverAddr));
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(port);
    serverAddr.sin_addr.s_addr=inet_addr("127.0.0.1");

// now will set up a server for client to be connected so we bind the socket with client 
// by following connection 
    connection=bind(sockid,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    if (connection<0){
    //Sometimes binding issue shows because either the port either not closed due to previous connection or not extablished properly 
    printf("Server binding not successful...\n");
    exit;
    }
    else{
        printf("server with client binded properly....%d\n",5555);
    }
//Now listening of client will start from server 
//Here 4 mentioned in if statement to show how many clients we want to connect
//we can take as many clients as we want because we are forking 
    if(listen(sockid,4)==0){
        printf("server started listening......\n");
        exit;
    }
    else{
        printf("there is an error in listening...\n" );
    }
// Create infinite loop tp accept connection from server 
// Here we intiated one newsocket
   while(1){
    newSocket=accept(sockid,(struct sockaddr*)&newAddr,&addr_size);
    if(newSocket<0){
        exit;
    }
    
//print the ip and port of newsocket id inet_ntoa
//Below nthos is the function of short integer which convert into network byte to host byte 
    printf("Connection accepted %s:%d\n",inet_ntoa(newAddr.sin_addr),ntohs(newAddr.sin_port));
    if((childpid=fork())==0){
        close(sockid);
        while(1){
            recv(newSocket,buffer,1024,0);
            if(strcmp(buffer,":exit")==0){
                printf("disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr),ntohs(newAddr.sin_port));
                break;
            }
            else{
                printf("Client: %s\n",buffer);
                send(newSocket,buffer,strlen(buffer),0);
                bzero(buffer,sizeof(buffer));
            }

        }
    }
   
   } 
//newsocket loop close here after all connecting/disconnecting established 
   close(newSocket);  
    
   return 0;
  
}