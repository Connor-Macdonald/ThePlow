#include "physical_address_access.h"
#include "servo_drive_functions.h"
float sensor1_old
float sensor2_old
// NOTE: left and right must be opposite
int write_servo (int speed, volatile int *servo_pointer){
    //if *servo_pointer ==

    if (speed >= SPEED_HIGH){
        speed = SPEED_HIGH;
    }
    if( speed >= SPEED_LOW && speed <= SPEED_HIGH){
        // Success write speed
        *servo_pointer = speed;
        return 1;
    }
    // failure 
    else return 0;
}


float read_servo_pos (volatile int *encoder_pointer) {
    int unitsFC = 360;
    float high_micro = (*encoder_pointer) / 50;
    float duty_cycle = 100 * (high_micro / ENCODER_PERIOD);
    float theta = ((duty_cycle - DUTY_CYCLE_MIN) * CIRCLE_UNITS) / (DUTY_CYCLE_MAX - DUTY_CYCLE_MIN + 1);
    printf("angle: %f\n",theta); 
    if(theta < 0){
        theta = 0;
    } 
    else if(theta > (unitsFC - 1)) theta = unitsFC - 1;
    return theta;
}

float read_servo_pos_outlier(volatile int *encoder_pointer, int sensor){
    float current_sensor = read_servo_pos(encoder_pointer);
    if(sensor == 1){
        if(abs(current_sensor - sensor1_old) > OUTLIER_THRESHOLD)
            return sensor1_old
        else 
            sensor1_old = current_sensor;
    }
    else if(sensor == 2){
        if(abs(current_sensor - sensor2_old) > OUTLIER_THRESHOLD)
            return sensor2_old
        else 
            sensor2_old = current_sensor;
    }
}

/*
void drive_straight (int inpspeed){
    float theta_r = read_servo_pos(right_servo_encoder);
    float theta_l = read_servo_pos(left_servo_encoder);
    delay(20); // delay equivalent to the write freq of control sig of servo @ 50 Hz

    // need to deal with edge cases where transition from 360 - 0

    CIRCLE_UNITS

    float theta_r_diff = read_servo_pos(right_servo_encoder) - theta_r;
    float theta_l_diff = read_servo_pos(left_servo_encoder) - theta_l;
    printf("angle diff right: %f", theta_r_diff);
    printf("angle diff left: %f", theta_l_diff)

    if theta_r_diff > 300 && theta_r


    // set speed difference to be proportionate to difference between wheels
    if (theta_r_diff > theta_l_diff){
        float speed_multiplier = 100 * (theta_r_diff - theta_l_diff) / theta_l_diff;
        int r_speed = inpspeed * (1 + speed_multiplier*0.3);
    }
    else{
        float speed_multiplier = (theta_l_diff - theta_r_diff) / theta_r_diff;
        int r_speed = inpspeed * (1 - speed_multiplier*0.3);
    }

    write_servo(-inpspeed, left_servo);
    write_servo(r_speed, right_servo);
}
*/




/*
// first input is the pointer to the wheel which is the direction of turning
// second input is the pointer to the other wheel

void smart_turn(volatile int turn_dir, volatile int other_dir){
    // complete an arcing turn in the turning direction
    // push snow out of the way off the wall
    // complete 90 degree turn
    // continue forward
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
   // read_servo_pos(); // get where it is pointing now
   // read_gyro(&x_raw, &y_raw, &z_raw);
    // some shitty math
    //write_servo_angle();
}

int write_servo_angle(volatile int servo_pointer){
    // ya boi doesn't know how to do this
}

int turn_n_degrees(volatile int turn_dir, volatile int off_dir, int numDegrees){
    if(numDegrees < 0 || numDegrees > 360){
	return 0;
    }else{
	write_servo(4,off_dir);
	write_servo(1,turn_dir);
	// continue turning until the encoders read the right angles
	sleep(1);
	//while((*left_encoder) && (*right_encoder));
	write_servo(0,turn_dir);
	write_servo(0,off_dir);
    }
    return 1;
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

// pivot on wheel (1 front left, 2 front right, 3 back left, 4 back right)

*/