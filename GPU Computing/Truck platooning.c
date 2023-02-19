#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <cuda_runtime.h>

#define DISTANCE_THRESHOLD 10 // Distance threshold between two trucks in meters
#define ACCELERATION 0.5 // Acceleration of the trucks in m/s^2
#define MAX_SPEED 20 // Maximum speed of the trucks in m/s

typedef struct Truck {
    int id;
    double position;
    double speed;
    double acceleration;
} Truck;

__global__ void updateTruckKernel(Truck *truck, double time) {
    truck->speed += truck->acceleration * time;
    if (truck->speed > MAX_SPEED) {
        truck->speed = MAX_SPEED;
    }
    truck->position += truck->speed * time;
}

void updateTruck(Truck *truck, double time) {
    Truck *d_truck;
    cudaMalloc(&d_truck, sizeof(Truck));
    cudaMemcpy(d_truck, truck, sizeof(Truck), cudaMemcpyHostToDevice);
    updateTruckKernel<<<1, 1>>>(d_truck, time);
    cudaMemcpy(truck, d_truck, sizeof(Truck), cudaMemcpyDeviceToHost);
    cudaFree(d_truck);
}

int main() {
    Truck lead_truck = {1, 0.0, 0.0, ACCELERATION};
    Truck follow_truck = {2, 0.0, 0.0, ACCELERATION};
    
    while (true) {
        // Update the position and speed of the lead truck
        updateTruck(&lead_truck, 1.0);
        printf("Truck %d: position = %f m, speed = %f m/s, acceleration = %f m/s^2\n", 
               lead_truck.id, lead_truck.position, lead_truck.speed, lead_truck.acceleration);
        
        // Check the distance between the two trucks
        double distance = follow_truck.position - lead_truck.position;
        if (distance < DISTANCE_THRESHOLD) {
            // Update the speed of the following truck to match the lead truck
            follow_truck.speed = lead_truck.speed;
        } else {
            // Update the acceleration of the following truck to catch up with the lead truck
            follow_truck.acceleration = ACCELERATION;
            updateTruck(&follow_truck, 1.0);
        }
        
        // Update the position of the following truck
        updateTruck(&follow_truck, 1.0);
        printf("Truck %d: position = %f m, speed = %f m/s, acceleration = %f m/s^2\n", 
               follow_truck.id, follow_truck.position, follow_truck.speed, follow_truck.acceleration);
    }
    
    return 0;
}
