#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "address_map_arm.h"
#include "physical_address_access.h"
#include "servo_drive_functions.h"
#include "sensor_functions.h"
// TODO: Add to address map
#define RIGHT_SERVO 0x00000010
#define LEFT_SERVO 0x00000018
#define RIGHT_SERVO_ENCODER 0x00000020
#define LEFT_SERVO_ENCODER 0x00000028
#define DIST_SENSOR_1 0x00000030
#define DIST_SENSOR_2 0x00000038


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
    volatile int * right_servo = (int *) (LW_virtual + RIGHT_SERVO); 
    volatile int * left_servo = (int *) (LW_virtual + LEFT_SERVO); 
    volatile int * right_servo_encoder = (int *) (LW_virtual + RIGHT_SERVO_ENCODER); 
    volatile int * left_servo_encoder = (int *) (LW_virtual + LEFT_SERVO_ENCODER); 
    volatile int * dist_1 = (int *) (LW_virtual + DIST_SENSOR_1); 
    volatile int * dist_2 = (int *) (LW_virtual + DIST_SENSOR_1); 
    
    // Both servos max speed forward
    write_servo(4, right_servo);
    write_servo(4, left_servo);
	
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





