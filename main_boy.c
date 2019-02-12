#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include "address_map_arm.h"
#include "physical_address_access.h"
#include "physical_address_access.c"
#include "servo_drive_functions.c"
#include "servo_drive_functions.h"
#include "sensor_functions.h"
#include "sensor_functions.c"


// TODO: Add to address map
#define RIGHT_SERVO 0x00000010
#define LEFT_SERVO 0x00000018
#define RIGHT_SERVO_ENCODER 0x00000020
#define LEFT_SERVO_ENCODER 0x00000030
#define DIST_SENSOR_1 0x00000040
#define DIST_SENSOR_2 0x00000048


//struct globalvariable 
//{
  //  /* data */
    //int *xDtata = 0;
    //int *yData = 0;
//}; 
/*
compile command is gcc thread.c -thread

Good thread tutorial for pthread.h
https://randu.org/tutorials/threads/

Tutorial on how to pass multiple arguments in threads
http://www.cse.cuhk.edu.hk/~ericlo/teaching/os/lab/9-PThread/Pass.html

Random thread function for testing
void* is used when working with different pointer types
*/
void *routing(void *num){
    int *x = (int *)num;
    printf("Thread 1 works! %d\n", *x);
    /*
    while(1){
        //int *xData = 0xff7300;
        //int *yData = 0xff7310;
        printf("Thread 1 works! %d\n", num);
    }
    */
}

void *positionCalc(void *num2){
    int *x = (int *)num2;
    printf("Thread 2 works! %d\n", *x);
    /*
    while(1){

        //write to memory
        *xdata = 300; 
        *ydata = 300;
    }
    */
}

void *sensorPos(void *num3){
    int *x = (int *)num3;
    printf("Thread 3 works! %d\n", *x);
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
    volatile int * right_servo = (int *) (LW_virtual + LEFT_SERVO); 
    volatile int * left_servo = (int *) (LW_virtual + RIGHT_SERVO); 
    volatile int * right_servo_encoder = (int *) (LW_virtual + RIGHT_SERVO_ENCODER); 
    volatile int * left_servo_encoder = (int *) (LW_virtual + LEFT_SERVO_ENCODER); 
    volatile int * dist_1 = (int *) (LW_virtual + DIST_SENSOR_1); 
    volatile int * dist_2 = (int *) (LW_virtual + DIST_SENSOR_2);

    /*
    //this code makes it go straight
    /*
    printf("deeeeeeeeeeeeeez nuts");
	int i =0;
	for (i = 0; i<1000;i++){
		    // Both servos max speed forward
		write_servo(0, right_servo);
		write_servo(0, left_servo);
		printf("Reading greater than 3 cm\n");
	}
	*/

    /*
    write_servo(0, right_servo);
    write_servo(0, left_servo);
    
    float theta_right = read_servo_pos(right_servo_encoder);
   	float theta_left = read_servo_pos(left_servo_encoder);
	int i =0;
	for(i = 0;i<10;i++){
   		printf("theta_right= %f \n", theta_right);
   		printf("theta_left= %f \n", theta_left );
   		
   		sleep(2);
   		theta_right = read_servo_pos(right_servo_encoder);
   		theta_left = read_servo_pos(left_servo_encoder);
   	}
	*/
/*
while(1){
    float dist1 = read_distance_sensor(dist_1);
    float dist2 = read_distance_sensor(dist_2);

    printf("Distance sensor #1: %f\n", dist1);
    printf("Distance sensor #2: %f\n", dist2);
    sleep(1);
}
*/



    /*The arguments required for pthread_create():
        pthread_t *thread: the actual thread object that contains pthread id
        pthread_attr_t *attr: attributes to apply to this thread
        void *(*start_routine)(void *): the function this thread executes
        void *arg: arguments to pass to thread function above
    */

    //NOTE: when compiling: gcc main_boy.c -o main_boy -lpthread

    //creating thread 1
	pthread_t thread1;
    int x =1;
    if(pthread_create(&thread1, NULL, routing, &x)){
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    if(pthread_join(thread1, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;

    }
    
    //creating thread 2
	pthread_t thread2;
    int y =2;
    if(pthread_create(&thread2, NULL, positionCalc, &y)){
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    if(pthread_join(thread2, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;

    }

   //creating thread 3
	pthread_t thread3;
    int z =3;
    if(pthread_create(&thread3, NULL, sensorPos, &z)){
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    if(pthread_join(thread3, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;

    }
    
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