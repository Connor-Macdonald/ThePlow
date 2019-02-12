#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "address_map_arm.h"
#include "physical_address_access.h"
#include "physical_address_access.c"
#include "servo_drive_functions.c"
#include "servo_drive_functions.h"
#include "sensor_functions.h"
#include "sensor_functions.c"
#include <pthread.h>

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

//compile command is gcc thread.c -thread

//Good thread tutorial for pthread.h
//https://randu.org/tutorials/threads/

//Tutorial on how to pass multiple arguments in threads
//http://www.cse.cuhk.edu.hk/~ericlo/teaching/os/lab/9-PThread/Pass.html

//Random thread function for testing
//void* is used when working with different pointer types
/*
void* routing(void* args){
    while(1){
        int *xData = 0xff7300;
        int *yData = 0xff7310;
    }
}

void* positionCalc(void* args){
    while(1){

        //write to memory
        *xdata = 300; 
        *ydata = 300;
    }
}

void* sensorPos(void* args){

}
*/

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

    //int dist1 = read_sensor(dist_1); // read at the beginning of the run
    //int dist2 = read_sensor(dist_2); // read at the beginning of the run
    
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
while(1){
    float dist1 = read_distance_sensor(dist_1);
    float dist2 = read_distance_sensor(dist_2);

    printf("Distance sensor #1: %f\n", dist1);
    printf("Distance sensor #2: %f\n", dist2);
    sleep(1);
}

    /*The arguments required for pthread_create():
        pthread_t *thread: the actual thread object that contains pthread id
        pthread_attr_t *attr: attributes to apply to this thread
        void *(*start_routine)(void *): the function this thread executes
        void *arg: arguments to pass to thread function above
*/
    //creating thread
	/*
    int t1 = pthread_create(pthread_t *thread, NULL,&routing,NULL)
    if(t1==0){
        printf("thread created");
    }else{
        printf("thread not created");
        return 0;
    }

      int t2 = pthread_create(pthread_t *thread, NULL,&positionCalc,NULL)
    if(t2==0){
        printf("thread created");
    }else{
        printf("thread not created");
        return 0;
    }
	    int t3 = pthread_create(pthread_t *thread, NULL,&sensorPos,NULL)
    if(t3==0){
        printf("thread created");
    }else{
        printf("thread not created");
        return 0;
    }
	*/
    
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