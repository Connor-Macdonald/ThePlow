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
    return theta;
}


void smart_turn_right(volatile int left_pointer, volatile int right_pointer){
    smart_turn(right_pointer,left_pointer);
}

void smart_turn_left(volatile int left_pointer, volatile int right_pointer){
    smart_turn(left_pointer, right_pointer);
}

void smart_turn(volatile int turn_dir, volatile int other_dir){
    write_servo(3,other_dir);
    write_servo(1,turn_dir);
    sleep(1);
    write_servo(1, other_dir);
    write_servo(1, turn_dir);
    sleep(1);
    write_servo(3,other_dir);
    write_servo(0,turn_dir);
    sleep(1);
    write_servo(4,turn_dir);
    write_servo(4,other_dir);
}

int gyro_read_and_adjust(volatile int gyro_addr, volatile int turn_pointer){
    float x_raw, y_raw, z_raw;
    read_servo_pos(); // get where it is pointing now
    read_gyro(&x_raw, &y_raw, &z_raw);
    // some shitty math
    write_servo_angle();
}

int write_servo_angle(volatile int servo_pointer){
    // ya boi doesn't know how to do this
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
