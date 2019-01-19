#include "physical_address_access.h"

int write_servo (int speed, volatile int servo_pointer){
    if( speed > SPEED_LOW && speed < SPEED_HIGH){
        // Success write speed
        *servo_pointer = speed
        return 1
    }
    // failiure 
    else return 0
}

// forward_nseconds
	// left_motor_forward_nseconds
	// right motor_forward_nseconds

// reverse_nseconds
	// right_motor_reverse_nseconds
	// left_motor_reverse_nseconds

// turn_n_degrees
	// if degree > 0 turn left that much
	// if degree < 0 turn right that much

// pivot on wheel (1 front left, 2 front right, 3 back left, 4 back right)