#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "math.h"
#include "queue.c"
#include "address_map_arm.h"
#include "physical_address_access.h"
#include "physical_address_access.c"
#include "sensor_functions.h"
#include "sensor_functions.c"
#include "servo_drive_functions.c"
#include "servo_drive_functions.h"


// TODO: Add to address map
#define LEFT_SERVO 0x00000010
#define RIGHT_SERVO 0x00000018
#define RIGHT_SERVO_ENCODER 0x00000030
#define LEFT_SERVO_ENCODER 0x00000020
#define DIST_SENSOR_1 0x00000040
#define DIST_SENSOR_2 0x00000048

struct Dist_sensor
{
   /* data */
    volatile int *sideways_sensor;
    volatile int *backwards_sensor;
};

void *sensor_thread(void *sensors){
    struct Dist_sensor *thread_sensors = (struct Dist_sensor*)sensors;
    printf("IN THREAD");
    weighted_distance_sensor((*thread_sensors).sideways_sensor, (*thread_sensors).backwards_sensor);
}

int main(void) {
    printf("START\n");
    // Initialization of FPGA bridge
    int fd = -1; // used to open /dev/mem
    void *LW_virtual; // physical addresses for light-weight bridge
    // Create virtual memory access to the FPGA light-weight bridge
    if ((fd = open_physical(fd)) == -1) return (-1);
    if (!(LW_virtual = map_physical(fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN))) return (-1);

    // Initialize all the nessacary virtual address pointers
	volatile int * left_servo = (unsigned int *) (LW_virtual + LEFT_SERVO);
    volatile int * right_servo = (unsigned int *) (LW_virtual + RIGHT_SERVO);
    volatile int * left_servo_encoder = (int *) (LW_virtual + LEFT_SERVO_ENCODER);
    volatile int * right_servo_encoder = (int *) (LW_virtual + RIGHT_SERVO_ENCODER);
    volatile int * dist_1 = (int *) (LW_virtual + DIST_SENSOR_1);
    volatile int * dist_2 = (int *) (LW_virtual + DIST_SENSOR_2);
	volatile int * push_button = (int *) (LW_virtual + 0x00000050);

    struct Dist_sensor sensors; //pass both sensors to thread
    sensors.sideways_sensor = dist_1;
    sensors.backwards_sensor = dist_2;

    pthread_t thread1;
    printf("Starting thread\n");
    pthread_create(&thread1, NULL, sensor_thread, (void*) &sensors);

    write_servo(0, left_servo, 1);
    write_servo(0, right_servo, 0);
    while(1){
        if(*push_button){
            break;
        }
    }

    printf("Killing Thread\n");
    killWhile();
    if (pthread_join(thread1, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;
    }
    unmap_physical (LW_virtual, LW_BRIDGE_SPAN);
    close_physical (fd);
    return 0;



    // **************  MAIN CODE STRUCTURE  *******************//

     int driveway_length = 150; //how far it goes from wall
     int wall_limit = 20; //how close it gets to the wall before initiating turn
     int cutoff = 20; //distance from sidewall to stop running plow routine

     //wait for push button
     while (1) {
         if (*push_button) {
             break;
         }
     }
     sleep(2); //delay after button pressed

     while(1) {
         //drive straight until it stops
         straight_hardcode(left_servo, right_servo, 150);
         //check to see when to break
         if(query_weighted_distances(1) < cutoff){ break;}
         //quick transition to reverse
         fwd_to_rev(left_servo, right_servo);
         //drive reverse until set distance from wall
         straight_hardcode(left_servo, right_servo, 20);
         //right turn
         turn(left_servo_encoder, right_servo_encoder, left_servo, right_servo, 90, 0);
         //go straight for one second
         fwd_for_time(left_servo, right_servo, 1000000000);
         //left turn
         turn(left_servo_encoder, right_servo_encoder, left_servo, right_servo, 90, 1);
}

    /*// THREAD STUFF

	//The arguments required for pthread_create():
    //    pthread_t *thread: the actual thread object that contains pthread id
    //    pthread_attr_t *attr: attributes to apply to this thread
    //    void *(*start_routine)(void *): the function this thread executes
    //    void *arg: arguments to pass to thread function above

    //NOTE: when compiling: gcc main_boy.c -o main_boy -lpthread
    // Unmap FPGA bridge
    */
   
