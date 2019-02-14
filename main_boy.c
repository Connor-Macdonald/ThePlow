#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "address_map_arm.h"
#include "physical_address_access.h"
#include "physical_address_access.c"
#include "servo_drive_functions.c"
#include "servo_drive_functions.h"
#include "sensor_functions.h"
#include "sensor_functions.c"


// TODO: Add to address map
#define LEFT_SERVO 0x00000010
#define RIGHT_SERVO 0x00000018
#define RIGHT_SERVO_ENCODER 0x00000020
#define LEFT_SERVO_ENCODER 0x00000030
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
    volatile int *left_servo = (unsigned int *) (LW_virtual + LEFT_SERVO);
    volatile int *right_servo = (unsigned int *) (LW_virtual + RIGHT_SERVO);
    volatile int *left_servo_encoder = (int *) (LW_virtual + RIGHT_SERVO_ENCODER);
    volatile int *right_servo_encoder = (int *) (LW_virtual + LEFT_SERVO_ENCODER);
    volatile int *dist_1 = (int *) (LW_virtual + DIST_SENSOR_1);
    volatile int *dist_2 = (int *) (LW_virtual + DIST_SENSOR_2);
    volatile int *push_button = (int *) (LW_virtual + 0x00000050);

    struct Dist_sensor sensors; //pass both sensors to thread
    sensors.sideways_sensor = dist_1;
    sensors.backwards_sensor = dist_2;

    pthread_t thread1;
    printf("Starting thread\n");
    pthread_create(&thread1, NULL, sensor_thread, (void *) &sensors);
    while (1) {
        if (*push_button) {
            break;
        }
    }

    // turn(left_servo_encoder,right_servo_encoder,left_servo, right_servo, 320, 1);
    sleep(3);

    // Go Forward Distance
    while (1) {
        drive_straight(35, left_servo, right_servo, left_servo_encoder, right_servo_encoder);
        float rev_encoder = query_weighted_distances(2);
        float side_encoder = query_weighted_distances(1);
        printf("BACKDISTANCE: %f, SIDEDISTANCE: %f\n", rev_encoder, side_encoder);
        if (*push_button) {
            sleep(1);
            break;
        }
    }
    // Drive Backwards
    while (1) {
        drive_straight(-35, left_servo, right_servo, left_servo_encoder, right_servo_encoder);
        if (*push_button) {
            sleep(1);
            break;
        }
    }

    write_servo(0, left_servo, 1);
    write_servo(0, right_servo, 0);
    printf("Killing Thread\n");
    killWhile();
    if (pthread_join(thread1, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;
    }


    // **************  MAIN CODE STRUCTURE  *******************//

    //wait for push button
    while (1) {
        if (*push_button) {
            break;
        }
    }
    int driveway_length = 500;

    //drive straight until it stops
    drive_straight_ultrasonic (35, left_servo, right_servo, left_servo_encoder, right_servo_encoder, driveway_length)
    //add quick fwd then quick reverse function to spin wheel



















}
    /*// THREAD STUFF

	//The arguments required for pthread_create():
    //    pthread_t *thread: the actual thread object that contains pthread id
    //    pthread_attr_t *attr: attributes to apply to this thread
    //    void *(*start_routine)(void *): the function this thread executes
    //    void *arg: arguments to pass to thread function above

    //NOTE: when compiling: gcc main_boy.c -o main_boy -lpthread
    // Unmap FPGA bridge
    unmap_physical (LW_virtual, LW_BRIDGE_SPAN);
    close_physical (fd);
    return 0;
