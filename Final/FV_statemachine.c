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



typedef enum
 {   
    Running_Platoon,
    Leading_Vehicle,
    Intrusion_Detected,
    Check_Intrusion_Type,
    Captured_actual_Position,
    Intrusion_passed,
    Platoon_recovered_position,
    Final
 }Platoon_state;

 typedef enum{
    wait_for_communication,
    range_platoon,
    available,
    send_data_information,
} Platoon_Events;


  
void reduce_distance(int sock) {
    // Send message to server to reduce distance between two trucks
    char msg[] = "REDUCE_DISTANCE";
    if (send(sock, msg, strlen(msg), 0) < 0) {
        perror("Message send failed");
        exit(EXIT_FAILURE);
    }

    // Wait for response from server
    char response[1024];
    int bytes_received = recv(sock, response, 1024, 0);
    if (bytes_received < 0) {
        perror("Message receive failed");
        exit(EXIT_FAILURE);
    }
    response[bytes_received] = '\0';

    // Print server response
    printf("Server response: %s\n", response);
}

int main(){
   struct  sockaddr_in serverAddr;
   int valread,i;
   //char buffer[BUFFER_SIZE] = {0};
    char buffer[1024];
    //create new sock for client 
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
    //once it got connected to server we check the connection is established continuously

    //cretae message buffer 
    char msg_given[520];
    while (1){
        printf("Following vehicle send :..\t");
        scanf("%s",&msg_given[0]);
        send(sock,msg_given,strlen(msg_given),0);


    

    //also check here weather server is accepting the client check with below code 
    if(strcmp(msg_given, ":exit")==0)
    {
        close(sock);
        printf("Disconneted to with lead vehicle..\n.");
        exit;
    }
    printf("Lead vehicle accepted the request..\n");
    //close(sock);
    
   

//After receiving the message
    if(recv(sock,msg_given,520,0)<0){
        printf("Again isssue in receiving...dicontinued\n");
    }
    else{
        char msg_provide[]="yes received";
        printf("server send back to client: \t%s\n",msg_provide);
        }

    }
    Platoon_state state = Running_Platoon;
    while (state != Final) {
        // Check the current state of the state machine and handle the appropriate events
        switch (state) {
            case Running_Platoon:
                // Wait for a new connection from the server
                if (recv(sock, NULL, 0, MSG_PEEK | MSG_DONTWAIT) == 0) {
                    // The server has closed the connection
                    state = Final;
                    break;
                }

                // Wait for the user to enter the 'C' command to start the platoon
                printf("Type C command to enter RUNNING_PLATOON state: ");
                char cmd;
                scanf("%s", &cmd);
                if (cmd == 'C') {
                     //Transition to the RUNNING_PLATOON state
                    state = Leading_Vehicle;
                }
                break;

            case Leading_Vehicle:
                // Send a message to the server to request the current truck speeds and distance
                char buffer[1024] = {0};
                sprintf(buffer, "get_speeds_and_distance");
                if (send(sock, buffer, strlen(buffer), 0) == -1) {
                    printf("Error sending message to server\n");
                    state = Final;
                    break;
                }

                // Receive the response from the server
                int valread = read(sock, buffer, 1024);
                if (valread == -1) {
                    printf("Error receiving message from server\n");
                    state = Final;
                    break;
                } else if (valread == 0) {
                    // The server has closed the connection
                    state = Final;
                    break;
                }

                // Parse the response to get the current truck speeds and distance
                double truck1_speed, truck2_speed, current_distance;
                sscanf(buffer, "%lf %lf %lf", &truck1_speed, &truck2_speed, &current_distance);

                // Calculate the speed to maintain the target distance
                double target_distance = 50.0;
                double speed_diff = (current_distance - target_distance) / 10.0;
                double new_truck2_speed = truck1_speed - speed_diff;

                // Send a message to the server to set the new truck speeds
                sprintf(buffer, "%f %f", truck1_speed, new_truck2_speed);
                if (send(sock, buffer, strlen(buffer), 0) == -1) {
                    printf("Error sending message to server\n");
                    state = Final;
                    break;
                }
                printf("new truck also read\n");
        }
    }
    return 0;
}








