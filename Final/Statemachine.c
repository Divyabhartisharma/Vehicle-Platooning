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
#define MAX_MSG_LENGTH 256
#define MSG_CLIENT_TO_SERVER "CLIENT_MSG"
#define MSG_SERVER_TO_CLIENT "SERVER_MSG"



//Defined all the states here 
typedef enum
 {   
    Running_Platoon,
    Leading_Vehicle,
    Intrusion_Detected,
    Check_Intrusion_Type,
    Captured_actual_Position,
    Intrusion_passed,
    Platoon_recovered_position,
    Final,
 }Platoon_state;

 typedef enum{
    wait_for_communication,
    range_platoon,
    available,
    send_data_information,
} Platoon_Events;


void maintain_constant_speed(Platoon_state *state) {
    *state = Running_Platoon;
    // Maintain a constant speed within the platoon
    double target_speed = 80.0; // target speed in km/h
    double speed_diff = 5.0; // difference in speed to adjust by in km/h

    // Get the current speed of each truck
    double truck1_speed = 80.0; // km/h
    double truck2_speed = 75.0; // km/h

    // Calculate the speed difference between the two trucks
    double speed_difference = truck1_speed - truck2_speed;

    // If the speed difference is greater than the allowed difference, adjust the speed of the trailing truck
    if (speed_difference > speed_diff) {
        truck2_speed = truck1_speed - speed_diff;
    } else if (speed_difference < -speed_diff) {
        truck2_speed = truck1_speed + speed_diff;
    }

    // Log the new speeds
    printf("Truck 1 speed: %f km/h, Truck 2 speed: %f km/h\n", truck1_speed, truck2_speed);
}

void reduce_distance_between_trucks(Platoon_state *state) {
    *state=Running_Platoon;
    // Reduce the distance between two trucks by adjusting the speeds
    double target_distance = 50.0; // target distance between trucks in meters
    double speed_diff = 5.0; // difference in speed to adjust by in km/h
   
    // Get the current speed of each truck
    double truck1_speed = 80.0; // km/h
    double truck2_speed = 75.0; // km/h

    double current_distance = 100.0; // meters

   

    // Calculate the current distance between the trucks
    

    if (current_distance > target_distance) {
        // If the current distance is greater than the target distance, reduce the speed of the trailing truck
        truck2_speed -= speed_diff;
    } else {
        // If the current distance is less than the target distance, increase the speed of the trailing truck
        truck2_speed += speed_diff;
    }


    // Log the new speeds and distance
    printf("Truck 1 speed: %f km/h, Truck 2 speed: %f km/h, Distance: %f meters\n", truck1_speed, truck2_speed, current_distance);
}


int main()
{
    int server_sock, client_sock, c,valread;
    struct sockaddr_in server_addr, client_addr;
    char *response="Ackknowlede";
    char client_message[100];
    //char buffer[1024];
    char msg_buffer[MAX_MSG_LENGTH]={0};
    Platoon_state state=Running_Platoon;


    char buffer[1024];
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

    printf("Received message from client: %s\n", msg_buffer);
    printf("Following_vehicle_Connection accepted %s:%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
    if((childpid = fork())==0){
       close(server_sock); 
    
            recv(client_sock,buffer,1024,0);
            if(strcmp(buffer,":exit")==0){
                printf("disconnected from %s:%d\n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
                break;
            }
            else{
                printf("Client: %s\n",buffer);
                send(client_sock,buffer,strlen(buffer),0);
                bzero(buffer,sizeof(buffer));
                
            }
        
              
        //printf("Debugging:");
        //while(1){
            printf("testing if reaching to while loop");    
    Platoon_state state= Running_Platoon;
    int event;
        switch (state) {
            case Running_Platoon:
                // Wait for a new connection from a client
                if (event == available) {
                    // Transition to the RUNNING_PLATOON state
                    state = Running_Platoon;
                }
                break;
            case Leading_Vehicle:
                // Maintain a constant speed within the platoon
                maintain_constant_speed(&state);
                break;
            case Intrusion_Detected:
                // Handle an intrusion detection event
                printf("Intrusion detected!\n");
                // Transition to the WAIT_FOR_CONNECTION state
                state = Running_Platoon;
                break;
            default:
                // Invalid state
                printf("Invalid state\n");
                exit(EXIT_FAILURE);
                break;
        }
        //break;
               
                // switch (msg_buffer[0]) {
                //     case 'P':
                //         printf("Passing slower truck.\n");
                //         //pass_slower_truck(&state);
                //         break;
                //     case 'L':
                //         printf("Taking lead of the platoon.\n");
                //         //lead_platoon(&state);
                //         break;
                //     default:
                //         printf("Following lead truck.\n");
                //         //follow_lead_truck(&state);
                //         break;
                // }
            // //Respond to client

            //     break;
            // case Leading_Vehicle:
            //     switch (msg_buffer[0]) {
            //         case 'F':
            //             printf("Following lead truck.\n");
            //             follow_lead_truck(&state);
            //             break;
            //         default:
            //             printf("Passing slower truck.\n");
            //             pass_slower_truck(&state);
            //             break;
            //     }
            //     break;
            // case Intrusion_Detected:
            //     switch (msg_buffer[0]) {
            //         case 'F':
            //             printf("Following lead truck.\n");
            //             break;
            //         default:
            //             printf("Taking lead of the platoon.\n");
                       
            //             break;
                
    
        //break;
         
        // }
    }
    }
    
    close(client_sock);
    //close(server_sock);

    return 0;
    
}    
