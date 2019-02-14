#include "physical_address_access.h"
#include "servo_drive_functions.h"
#include "time.h"
#include "math.h"

float sensor1_old;
float sensor2_old;


int write_servo (int speed, volatile int *servo_pointer, int ifleft){ //reverses the direction of the right servo speed
    if (ifleft == 1){
        write_servo_direct(speed, servo_pointer);
        return 1;
    }
    if (ifleft == 0){
        write_servo_direct(-speed,servo_pointer);
        return 1;
    }
    return 0;
}


int write_servo_direct (int speed, volatile int *servo_pointer){ //writes to the servo

    if (speed >= SPEED_HIGH){
        speed = SPEED_HIGH;
    }
    if (speed <= SPEED_LOW){
        speed = SPEED_LOW;
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
    float high_micro = (*encoder_pointer) / 100;
    float duty_cycle = 100 * (high_micro / ENCODER_PERIOD);
    float theta = ((duty_cycle - DUTY_CYCLE_MIN) * CIRCLE_UNITS) / (DUTY_CYCLE_MAX - DUTY_CYCLE_MIN + 1);
    if(theta < 0){
        theta = 0;
    } 
    else if(theta > (unitsFC - 1)) theta = unitsFC - 1;
    return theta;
}
// Dir = 1 -> forward
void turn(volatile int *left_servo_encoder, volatile int *right_servo_encoder, volatile int *left_servo, volatile int *right_servo, float targetChange, int dir){
    write_servo((dir ? 30 : -30),right_servo,0); // if the dir is 1, then we go fw, else bw
    //float encodeL = read_servo_pos(left_servo_encoder);
    float encodeR = read_servo_pos(right_servo_encoder);
    float prevEncode = encodeR;
    int targetAngle = ((int)targetChange + (int)encodeR)%360;
    int numPass = targetChange/360;
    printf("Current Encode: %f:     Targetting: %d\n",encodeR,targetAngle);
    do{
        while((encodeR >= (targetAngle + 5)) || (encodeR <= targetAngle - 5)){
            encodeR = read_servo_pos(right_servo_encoder);
            while(abs(encodeR-prevEncode) >= 4){ // ensure readings are accurate
                encodeR = read_servo_pos(right_servo_encoder);
                //                                      smmmMMMNNN updates every 1/800th of a second
                nanosleep((const struct timespec[]){{0, 0010000000L}}, NULL);
            }
            printf("Encoder 2 (right): %f\n",encodeR);
            nanosleep((const struct timespec[]){{0, 0010000000L}}, NULL);
            prevEncode = encodeR;
            
        }
        numPass--;
    }while(numPass >= 0);
	printf("Stopping wheels with reading %f\n",encodeR);
    write_servo(0,left_servo,1);
    write_servo(0,right_servo,1);
}


float read_servo_pos_outlier(volatile int *encoder_pointer, int sensor){
    float current_sensor = read_servo_pos(encoder_pointer);
    if(sensor == 1){
        if(abs(current_sensor - sensor1_old) > OUTLIER_THRESHOLD)
            return sensor1_old;
        else 
            sensor1_old = current_sensor;
    }
    else if(sensor == 2){
        if(abs(current_sensor - sensor2_old) > OUTLIER_THRESHOLD)
            return sensor2_old;
        else 
            sensor2_old = current_sensor;
    }
}

int drive_straight (int inpspeed, int *left_servo, int *right_servo, int *left_servo_encoder, int *right_servo_encoder){
    float jerkiness = 0.4; //value between 0-1 to monitor how big driving adjustments are
    int r_speed; //right servo speed, to be adjusted later in function

    float theta_r = read_servo_pos(right_servo_encoder);
    float theta_l = read_servo_pos(left_servo_encoder);

    nanosleep((const struct timespec[]){{0, 50000000L}}, NULL); //delay of 50 milliseconds

    float theta_r2 = read_servo_pos(right_servo_encoder);
    float theta_l2 = read_servo_pos(left_servo_encoder);

    float theta_r_diff = abs(theta_r2 - theta_r);
    float theta_l_diff = abs(theta_l2 - theta_l);

    //too much difference in wheel angle indicates incorrect sensor readout, recall function
    if (theta_l_diff > 50 || theta_r_diff > 50){
        return 0;
    }

    //cases where there is a transition caused by end of rotation
    if(theta_r_diff > 100 && theta_r > 300){ //if the difference made a transition from 360-0
        theta_r_diff = CIRCLE_UNITS - theta_r + theta_r2;
    }
    if(theta_l_diff > 100 && theta_l > 300){ //if the difference made a transition from 360-0
        theta_l_diff = CIRCLE_UNITS - theta_l + theta_l2;
    }
    if(theta_r_diff > 100 && theta_r < 60){ //if the difference made a transition from 0-360
        theta_r_diff = CIRCLE_UNITS - theta_r2 + theta_r;
    }
    if(theta_l_diff > 100 && theta_l < 60){ //if the difference made a transition from 0-360
        theta_l_diff = CIRCLE_UNITS - theta_l2 + theta_l;
    }

    // set speed difference to be proportionate to difference between wheels
    if (theta_r_diff > theta_l_diff){
        float speed_multiplier = (theta_r_diff - theta_l_diff) / theta_l_diff;
        if(speed_multiplier >= 1){
            speed_multiplier = 1;
        }
        r_speed = inpspeed * (1 + speed_multiplier*jerkiness);
//        printf("right wheel SPED UP");
    }
    else{
        float speed_multiplier = (theta_l_diff - theta_r_diff) / theta_r_diff;
        if (speed_multiplier >= 1){ //so speed cannot be set to or below zero
            speed_multiplier = 0.9;
        }
        r_speed = inpspeed * (1 - speed_multiplier*jerkiness);
//        printf("right wheel slowed down\n");
    }
    
//    printf("R is: %f\tL is: %f\n", theta_r_diff, theta_l_diff);
    if(inpspeed >= 0){
        write_servo(inpspeed, left_servo, 1);
        write_servo(r_speed + 8, right_servo, 0);
    }else{
        write_servo(inpspeed, left_servo, 1);
        write_servo(r_speed - 8, right_servo, 0);
    }
    return (theta_l_diff + theta_r_diff) / 2;

}


/*void drive_straight_ultrasonic (int inpspeed, int *left_servo, int *right_servo, int *left_servo_encoder, int *right_servo_encoder, float stop_distance){
    float initial_lateral_dist = query_weighted_distances(1); //initial distance from wall
    float backward_dist;
    nanosleep((const struct timespec[]){{0, 100000000L}}, NULL); //delay of 100 milliseconds

    while(backward_dist < stop_distance){ //continue the loop until the plow is at the end of the driveway
        float backward_dist = query_weighted_distances(2); //senses when to break out of loop
        int distance_sum = 0; //distance travelled since last query of distance from edge of driveway
        int i = 0;
        //drive for one second using encoder to correct direction
        for(i = 0; i < 20; i++){ //loop 20 times, 50ms per iteration, equal to 1 second
            distance_sum += drive_straight(35, left_servo, right_servo, left_servo_encoder, right_servo_encoder);
        }
        //float distance_travelled = WHEEL_CIRCUMFERENCE * distance_sum; //distance travelled in cm
        float lateral_dist_change = query_weighted_distances(1) - initial_lateral_dist; //amount of lateral change over one second drive
        float distance_travelled = query_weighted_distances(2) - backward_dist;
        //lateral distance change negative - bot has drifted right
        //lateral distance change positive - bot has drifted left
        float tan_ratio = lateral_dist_change / distance_travelled;
        float angle_deg = (atan(tan_ratio) * 180) / PI;
        printf("the angle being drive is: %f", angle_deg);
        //correction of direction, assumes turn function changes BOT DIRECTION by ANGLE input
        turn(left_servo_encoder, right_servo_encoder, left_servo, right_servo, -angle_deg);
    }
}*/



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