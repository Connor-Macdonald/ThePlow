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


void testSpeeds(volatile int *left_servo, volatile int *right_servo, volatile int *push_button){

    //RESULTS 
    // So far L:35 R:43 provides the straightest line
    // right should be 8 bigger 
    sleep(2);
    write_servo(70, left_servo, 1);
    write_servo(78, right_servo, 0);
    while(1){
        if(*push_button){
            break;
        }
    }
    write_servo(0, left_servo, 1);
    write_servo(0, right_servo, 0);
    sleep(2);
    write_servo(70, left_servo, 1);
    write_servo(79, right_servo, 0);
    while(1){
        if(*push_button){
            break;
        }
    }
    write_servo(0, left_servo, 1);
    write_servo(0, right_servo, 0);
    sleep(2);
    write_servo(70, left_servo, 1);
    write_servo(80, right_servo, 0);
    while(1){
        if(*push_button){
            break;
        }
    }
    write_servo(0, left_servo, 1);
    write_servo(0, right_servo, 0);
    sleep(2);
    write_servo(70, left_servo, 1);
    write_servo(81, right_servo, 0);
    while(1){
        if(*push_button){
            break;
        }
    }
    write_servo(0, left_servo, 1);
    write_servo(0, right_servo, 0);
}
struct globalvariable 
{
   /* data */
    int *xDtata = 0;
    int *yData = 0;
}; 
/*
compile command is gcc thread.c -thread

Good thread tutorial for pthread.h
https://randu.org/tutorials/threads/

Tutorial on how to pass multiple arguments in threads
http://www.cse.cuhk.edu.hk/~ericlo/teaching/os/lab/9-PThread/Pass.html

Random thread function for testing
void* is used when working with different pointer types
*/
void *sensor_thread(void *num){
    //int *p = (int *)num;
    int i =0;
    //printf("im here in thread 1\n");
    for(i =0;i<50;i++){
        x = i;
    printf("Thread 1 works! %d\n", x);
    /*
    while(1){
        //int *xData = 0xff7300;
        //int *yData = 0xff7310;
        printf("Thread 1 works! %d\n", num);
    }
    */
   sleep(1);
    }
}


int main(void)
{
    // Initialization of FPGA bridge
    int fd = -1; // used to open /dev/mem
    void *LW_virtual; // physical addresses for light-weight bridge
    // Create virtual memory access to the FPGA light-weight bridge
    if ((fd = open_physical (fd)) == -1) return (-1);
    if (!(LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN))) return (-1);

    // Initialize all the nessacary virtual address pointers
	volatile int * left_servo = (unsigned int *) (LW_virtual + LEFT_SERVO);
    volatile int * right_servo = (unsigned int *) (LW_virtual + RIGHT_SERVO);
    volatile int * right_servo_encoder = (int *) (LW_virtual + RIGHT_SERVO_ENCODER); 
    volatile int * left_servo_encoder = (int *) (LW_virtual + LEFT_SERVO_ENCODER); 
    volatile int * dist_1 = (int *) (LW_virtual + DIST_SENSOR_1); 
    volatile int * dist_2 = (int *) (LW_virtual + DIST_SENSOR_2);
	volatile int * push_button = (int *) (LW_virtual + 0x00000050); 
	
	while(1){
        if(*push_button){
            break;
        }
    }

    printf("Testing turning 90 CCW");
    turn(left_servo_encoder,right_servo_encoder,left_servo, right_servo, 90, 1);
    printf("Done");

    sleep(3);

    printf("Testing turning 90 CW");
    turn(left_servo_encoder,right_servo_encoder,left_servo, right_servo, 90, 0);
    printf("Done");

    sleep(3);

    printf("Testing turning 180 CCW");
    turn(left_servo_encoder,right_servo_encoder,left_servo, right_servo, 180, 1);
    printf("Done");

    sleep(3);

    printf("Testing turning 180 CW");
    turn(left_servo_encoder,right_servo_encoder,left_servo, right_servo, 180, 0);
    printf("Done");
    
    sleep(3);

    printf("Testing turning 360 CCW");
    turn(left_servo_encoder,right_servo_encoder,left_servo, right_servo, 360, 1);
    printf("Done");

    sleep(3);

    printf("Testing turning 360 CW");
    turn(left_servo_encoder,right_servo_encoder,left_servo, right_servo, 360, 0);
    printf("Done");
    
    // turn_right(left_servo_encoder, right_servo_encoder,left_servo, right_servo);
     while(1){
         drive_straight(35, left_servo, right_servo, left_servo_encoder, right_servo_encoder);
    }
    //testSpeeds(left_servo, right_servo, push_button);
    // turn_right(left_servo_encoder, right_servo_encoder,left_servo, right_servo);
//STARTING TO COMMENT OUT HERE
    /* Code to make it move forward
    TODO: place into function
    write_servo(0, right_servo);
    write_servo(0, left_servo);
   
    Reads distance sensor
    TODO: place into function.
    while(1){
        float dist1 = read_distance_sensor(dist_1);
        float dist2 = read_distance_sensor(dist_2);

        printf("Distance sensor #1: %f\n", dist1);
        printf("Distance sensor #2: %f\n", dist2);
        sleep(1);
    }
    ENDING COMMENT HERE */
   

    /*// THREAD STUFF

	//The arguments required for pthread_create():
    //    pthread_t *thread: the actual thread object that contains pthread id
    //    pthread_attr_t *attr: attributes to apply to this thread
    //    void *(*start_routine)(void *): the function this thread executes
    //    void *arg: arguments to pass to thread function above
    
    //NOTE: when compiling: gcc main_boy.c -o main_boy -lpthread
    printf("calling 1\n");
    //creating thread 1
	pthread_t thread1;
    pthread_create(&thread1, NULL, routing, NULL);
    printf("calling 2\n");
    //creating thread 2
	pthread_t thread2;
    pthread_create(&thread2, NULL, positionCalc, NULL);

   printf("calling 3\n");
   //creating thread 3
   //creating thread 3
	pthread_t thread3;
    int z =3;
    pthread_create(&thread3, NULL, sensorPos, &z);

    if(pthread_join(thread1, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;

    }
    if(pthread_join(thread2, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;

    }
      if(pthread_join(thread3, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;

    }*/
    
    // Unmap FPGA bridge
    unmap_physical (LW_virtual, LW_BRIDGE_SPAN);
    close_physical (fd);
    return 0;

   
}

/* #1 Possible Routing w/ 1 angled blade pointing right (for example)
	|---------------------------------------|
	|			v	<						|
	|			|	^						|
	|			|	|						|
	|			|	|						|
	|			|	|						|
	|			|	|						|
	|			|	|						|
	|			|	|						|
	|			|	|						|
	|			|	|						|
	|			|	|						|
	|			|	|						|
	|			|	|						|
	|			|	|						|			
	|			>------^					|
	-----------------------------------------
	Basically making a spiral out from a middle center line.
	Always turning the opposite direction of the direction of the blade.
*/ 