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
    volatile int *sideways_sensor;
    volatile int *backwards_sensor;
};

struct Encoder_sensor{
    volatile int *left_encoder;
    volatile int *right_encoder;
};


//initialization of thread to query distance sensor
void *sensor_thread(void *sensors){
    struct Dist_sensor *thread_sensors = (struct Dist_sensor*)sensors;
    weighted_distance_sensor((*thread_sensors).sideways_sensor, (*thread_sensors).backwards_sensor);
}

//initialization of thread to read servo encoders
void *encoderThread(void *encoder){
    struct Encoder_sensor *thread_encoder = (struct Encoder_sensor*)encoder;
    printf("IN THREAD");
    weighted_encoder_fb((*thread_encoder).left_encoder, (*thread_encoder).right_encoder);
}

int main(void) {
    // Initialization of FPGA bridge
    int fd = -1; // used to open /dev/mem
    void *LW_virtual; // physical addresses for light-weight bridge
    // Create virtual memory access to the FPGA light-weight bridge
    if ((fd = open_physical(fd)) == -1) return (-1);
    if (!(LW_virtual = map_physical(fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN))) return (-1);

    // Initialize all the necessary virtual address pointers
    volatile int *left_servo = (unsigned int *) (LW_virtual + LEFT_SERVO);
    volatile int *right_servo = (unsigned int *) (LW_virtual + RIGHT_SERVO);
    volatile int *left_servo_encoder = (int *) (LW_virtual + LEFT_SERVO_ENCODER);
    volatile int *right_servo_encoder = (int *) (LW_virtual + RIGHT_SERVO_ENCODER);
    volatile int *dist_1 = (int *) (LW_virtual + DIST_SENSOR_1);
    volatile int *dist_2 = (int *) (LW_virtual + DIST_SENSOR_2);
    volatile int *push_button = (int *) (LW_virtual + 0x00000050);

    //pass both sensors to thread
    struct Dist_sensor sensors; 
    sensors.sideways_sensor = dist_1;
    sensors.backwards_sensor = dist_2;

    //pass both sensors to thread for encoders
    struct Encoder_sensor encoder;
    encoder.left_encoder = left_servo_encoder;
    encoder.right_encoder = right_servo_encoder;

    //make sure the bot is initially at a standstill
    write_servo(0, left_servo, 1);
    write_servo(0, right_servo, 0);

    //create thread 1
    pthread_t thread1;
    pthread_create(&thread1, NULL, sensor_thread, (void *) &sensors);

    //create thread 2
    pthread_t thread2;
    pthread_create(&thread2, NULL, encoderThread, (void *) &encoder);


    // **************  MAIN CODE STRUCTURE  *******************//
    int driveway_length = 150; //how far it goes from wall, in cm
    int wall_limit = 20; //how close it gets to the wall before initiating turn, in cm
    int cutoff = 20; //distance from sidewall to stop running plow routine, in cm

    //wait for push button
    while(1){
        sleep(1);
        write_servo(0, left_servo, 1);
        write_servo(0, right_servo, 0);
        while (1) {
            if (*push_button==1) {
                break;
            }
            if (*push_button >= 2){
                return 0;
            }
        }
        sleep(1); //delay after button pressed
        while (1) {
            //drive forward function with ultrasonic turn correction
            void drive_straight_ultrasonic (30, *left_servo, *right_servo, *left_servo_encoder, *right_servo_encoder, 150)
            if (query_weighted_distances(1) < cutoff) { break; }
            //quick transition to reverse to push snow off the end of the driveway
            fwd_to_rev(left_servo, right_servo);
            //drive reverse until set distance from wall with turn correction
            void drive_straight_ultrasonic (-30, *left_servo, *right_servo, *left_servo_encoder, *right_servo_encoder, 150)
            //right turn 90 degrees
            void turn(*left_servo_encoder, *right_servo_encoder, *left_servo, *right_servo, 90, 1)
            //go forward to setup next plow iteration
            write_servo(30, left_servo, 1);
            write_servo(38, right_servo, 0);
            sleep(2);
            //left turn 90 degrees
            void turn(*left_servo_encoder, *right_servo_encoder, *left_servo, *right_servo, 90, 0)


            write_servo(30, left_servo, 1); //stop bot
            write_servo(38, right_servo, 0);
            nanosleep((const struct timespec[]){{1, 999999999}}, NULL); //left turn

            //breaking the out of the loop
            if (*push_button) {
                while(!*push_button) {}
                    break;
            }
        }

        //break out of function
         if (*push_button) {
                while(!*push_button) {}
                    break;
         }
    }

    //Stopping plow
    write_servo(0, left_servo, 1);
    write_servo(0, right_servo, 0);

    //kill thread
    killWhile();
    if (pthread_join(thread1, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;
    }

    //close bridge
    unmap_physical(LW_virtual, LW_BRIDGE_SPAN);
    close_physical(fd);
    return 0;
}
