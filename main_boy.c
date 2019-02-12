#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "address_map_arm.h"
#include "physical_address_access.h"
#include "servo_drive_functions.h"
#include "sensor_functions.h"
#include <pthread.h>

// TODO: Add to address map
#define RIGHT_SERVO 0x00000010
#define LEFT_SERVO 0x00000018
#define RIGHT_SERVO_ENCODER 0x00000020
#define LEFT_SERVO_ENCODER 0x00000030
#define DIST_SENSOR_1 0x00000040
#define DIST_SENSOR_2 0x00000048

struct globalvariable 
{
    /* data */
    int *xDtata = 0;
    int *yData = 0;
};


//compile command is gcc thread.c -thread

//Good thread tutorial for pthread.h
//https://randu.org/tutorials/threads/

//Tutorial on how to pass multiple arguments in threads
//http://www.cse.cuhk.edu.hk/~ericlo/teaching/os/lab/9-PThread/Pass.html

//Random thread function for testing
//void* is used when working with different pointer types
void* routing(void* args){
    while(1){
        int *xData = 0xff7300;
        int *yData = 0xff7300;
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

int main(void)
{
    // Initialization of FPGA bridge
    int fd = -1; // used to open /dev/mem
    void *LW_virtual; // physical addresses for light-weight bridge
    // Create virtual memory access to the FPGA light-weight bridge
    if ((fd = open_physical (fd)) == -1)
    return (-1);
    if (!(LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)))
    return (-1);
    // Initialize all the nessacary virtual address pointers
    volatile int * right_servo = (int *) (LW_virtual + RIGHT_SERVO); // Write value -16 10 16
    volatile int * left_servo = (int *) (LW_virtual + LEFT_SERVO);  // note left and right will be oposite
    volatile int * right_servo_encoder = (int *) (LW_virtual + RIGHT_SERVO_ENCODER);  // Use read_servo_pos to get angle  
    volatile int * left_servo_encoder = (int *) (LW_virtual + LEFT_SERVO_ENCODER); // note left and right will be oposite
    volatile int * dist_1 = (int *) (LW_virtual + DIST_SENSOR_1); // Read using read_distance_sensor to get cm
    volatile int * dist_2 = (int *) (LW_virtual + DIST_SENSOR_2);

    int maxForward = read_distance_sensor(dist_1); // read at the beginning of the run
    int maxSide = read_distance_sensor(dist_2); // read at the beginning of the run
    
    // Both servos max speed forward
    write_servo(16, right_servo);
    write_servo(16, left_servo);
	// Start algorithm here #1
	
	/*
	while (still snow to plow)
		while (no wall){
			go forward
		}
		turn left (assuming blade is facing right) 90
		forward x amount (to pass the path of the previous pass)
		turn left 90
	}
	*/
    	
    // Algorithm #2 (the one we're using)
	
	/* turnDir = 1;
	 * while(still snow to plow){
	 * 	while(no wall){
	 * 		go forward;
	 * 	}
	 * 	if(turnDir % 2 == 1){
	 * 		*smart turn left;
	 * 	}else{
	 * 		*smart turn right;
	 * 	}
	 * }
	 */
    int forward_dist = read_distance_sensor(dist_1);
    int side_dist = read_distance_sensor(dist_2);
    int direction = 1;

    while(side_dist > 150){ // change this value
    	while(forward_dist > 150){ // experimentally determined distance for turning radius
            forward_dist = read_distance_sensor(dist_1);
    	}
        if(direction % 2 == 0){
	    smart_turn_right();
	}else{
	    smart_turn_left();
	}
	direction++;
    }
	



    /*The arguments required for pthread_create():
        pthread_t *thread: the actual thread object that contains pthread id
        pthread_attr_t *attr: attributes to apply to this thread
        void *(*start_routine)(void *): the function this thread executes
        void *arg: arguments to pass to thread function above
*/
    //creating thread
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
	    int t3 = pthread_create(pthread_t *thread, NULL,&writeMem,NULL)
    if(t3==0){
        printf("thread created");
    }else{
        printf("thread not created");
        return 0;
    }


    return 0;
    
    
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




