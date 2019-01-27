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

int read_servo_pos (volatile int encoder_pointer) {
    int duty_cycle = *encoder_pointer;
    int theta = ((duty_cycle - DUTY_CYCLE_MIN) * CIRCLE_UNITS) / (DUTY_CYCLE_MAX - DUTY_CYCLE_MIN + 1); 
    if(theta < 0) theta = 0;
    else if(theta > (unitsFC - 1)) theta = unitsFC - 1;
    return theta
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