#include "physical_address_access.h"

// NOTE: left and right must be opposite
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
    int high_micro = *encoder_pointer / 100;
    int duty_cycle = 100 * (high_micro / ENCODER_PERIOD)
    int theta = ((duty_cycle - DUTY_CYCLE_MIN) * CIRCLE_UNITS) / (DUTY_CYCLE_MAX - DUTY_CYCLE_MIN + 1); 
    if(theta < 0) theta = 0;
    else if(theta > (unitsFC - 1)) theta = unitsFC - 1;
    return theta
}


int smart_turn_right(volatile int left_pointer, volatile int right_pointer){
    write_servo(3,left_pointer); // 90 degree forward/rotating turn
    write_servo(1,right_pointer);
    // wait x amount of time, now perpendicular to wall
    sleep(1);
    write_servo(1,left_pointer);
    write_servo(1,right_pointer);
    // pushed the snow out of the way
    sleep(1);
    write_servo(3, left_pointer); // Rotate on the spot
    write_servo(0, right_pointer);
    sleep(1);
    write_servo(4, left_pointer);
    write_servo(4,right_pointer);
}

int smart_turn_left(volatile int left_pointer, volatile int right_pointer){
    write_servo(1,left_pointer);
    write_servo(3,right_pointer);
    sleep(1);
    write_servo(1,left_pointer);
    write_servo(1,right_pointer);
    sleep(1);
    write_servo(1,left_pointer);
    write_servo(3,right_pointer);
    sleep(1);
    write_servo(4,left_pointer);
    write_servo(4,right_pointer);
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
=======
// pivot on wheel (1 front left, 2 front right, 3 back left, 4 back right)
>>>>>>> 427fe829368c57a87647a7fe57dc038de31a4941
