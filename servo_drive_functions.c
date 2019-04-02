#include "physical_address_access.h"
#include "servo_drive_functions.h"


float sensor1_old;
float sensor2_old;


int write_servo (int speed, volatile int *servo_pointer, int ifleft){
    //ifleft flips the direction of the right servo speed when true
    //this allows a positive value to move either servo in the forward direction
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

float babs(float first, float second){
    if (first > second){
        return (first - second);
    }else{
        return (second - first);
    }
}


//this function turns the bot either right or left based on dir input
//the amount to turn is dictated by the variable targetChange
void turn(volatile int *left_servo_encoder,
          volatile int *right_servo_encoder, 
          volatile int *left_servo, 
          volatile int *right_servo, 
          float targetChange, 
          int dir){
    //dir 1 -> forward
    write_servo(((dir == 1) ? 38 : -30),right_servo,0); // if the dir is 1, then we go fw, else bw
    float encodeR;
    float prevEncode = read_servo_pos(right_servo_encoder);
    float diff = 0.0;
    while(diff < targetChange){

        encodeR = read_servo_pos(right_servo_encoder);
        while((babs(encodeR,prevEncode) >= 4) && (babs(encodeR,prevEncode) <= 345)){ // ensure readings are accurate
            encodeR = read_servo_pos(right_servo_encoder);
            //                                      smmmMMMNNN updates every 1/800th of a second
            nanosleep((const struct timespec[]){{0, 0001000000L}}, NULL);
        }
        nanosleep((const struct timespec[]){{0, 0001000000L}}, NULL);
        printf("%.3f    %.3f",encodeR,prevEncode);
        if (encodeR < 60 && prevEncode > 300){
            diff += encodeR + (360.0 - prevEncode);
        }else if (encodeR > 300 && prevEncode < 60){
            diff += prevEncode + (360.0 - encodeR);
        }else{
            diff += (babs(prevEncode,encodeR));
        }
        prevEncode = encodeR;
        printf("Difference: %f\n",diff);
    }
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


//drive straight function with correction based on wheel encoder feedback
int drive_straight (int inpspeed, volatile int *left_servo, volatile int *right_servo, volatile int *left_servo_encoder, volatile int *right_servo_encoder){
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
        printf("right wheel SPED UP");
    }
    else{
        float speed_multiplier = (theta_l_diff - theta_r_diff) / theta_r_diff;
        if (speed_multiplier >= 1){ //so speed cannot be set to or below zero
            speed_multiplier = 0.9;
        }
        r_speed = inpspeed * (1 - speed_multiplier*jerkiness);
        printf("right wheel slowed down\n");
    }
    write_servo(inpspeed, left_servo, 1);
    write_servo(r_speed + 8, right_servo, 0);
    return abs(theta_l_diff + theta_r_diff) / 2; //amount of change since last call
}


//drive straight with correction based on ultrasonic sensors
void drive_straight_ultrasonic (int inpspeed, int *left_servo, int *right_servo, int *left_servo_encoder, int *right_servo_encoder, float stop_distance){
     float initial_lateral_dist = query_weighted_distances(1); //initial distance from wall
     float backward_dist;
     nanosleep((const struct timespec[]){{0, 100000000L}}, NULL); //delay of 100 milliseconds

     while(backward_dist < stop_distance){ //continue the loop until the plow is at the end of the driveway
         int distance_sum = 0; //distance travelled since last query of distance from edge of driveway
         int i = 0;
         //drive for one second using encoder to correct direction
         for(i = 0; i < 20; i++){ //loop 20 times, 50ms per iteration, equal to 1 second
             distance_sum += drive_straight(inpspeed, left_servo, right_servo, left_servo_encoder, right_servo_encoder);
         }
         //float distance_travelled = WHEEL_CIRCUMFERENCE * distance_sum; //distance travelled in cm
         float lateral_dist_change = query_weighted_distances(1) - initial_lateral_dist; //amount of lateral change over one second drive
         float distance_travelled = query_weighted_distances(2) - backward_dist;
         //lateral distance change negative - bot has drifted right
         //lateral distance change positive - bot has drifted left
         float tan_ratio = lateral_dist_change / distance_travelled;
         float angle_deg = (atan((double)tan_ratio) * 180) / PI;
         //correction of direction, assumes turn function changes BOT DIRECTION by ANGLE input

         write_servo(0, left_servo, 1); //stop bot
         write_servo(0, right_servo, 0);
         sleep(1);
         turn(left_servo_encoder, right_servo_encoder, left_servo, right_servo, angle_deg, 0); //adjust angle

         if(angle_deg > 0){ //right turn
             int dir = 1;
             float time = angle_deg * (2.45 / 90);
         }else{ //left turn
             int dir = 0;
             float time = angle_deg * (1.72 / 90);
         }
         float turn(left_servo, right_servo, long time, int dir); //dir = 1 is right turn
         float backward_dist = query_weighted_distances(2); //senses when to break out of loop
     }
 }


 //used at the end of each snow push to quickly push the snow off the end of the driveway
void fwd_to_rev(int volatile *left_servo, int volatile *right_servo) {
    write_servo(80, left_servo, 1); //drive fwd
    write_servo(80, right_servo, 0);
    nanosleep((const struct timespec[]){{0, 500000000L}}, NULL); //delay of 500 milliseconds
    write_servo(-80, left_servo, 1); //drive rev
    write_servo(-80, right_servo, 0);
    nanosleep((const struct timespec[]){{0, 500000000L}}, NULL); //delay of 500 milliseconds
}

