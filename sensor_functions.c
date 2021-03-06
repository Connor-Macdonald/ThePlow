#include "sensor_functions.h"
//#include "queue.c"

int stopWhile = 1;

struct Queue* queue1;
struct Queue* queue2;
struct Queue* queue3; //left wheel queue
struct Queue* queue4; //right wheel queue
int queueNum = 15;
int old_side = 0;
int encoderQueueSize = 5;


//get ultrasonic distance sensor output in CM
float read_distance_sensor(volatile int *sensor_pointer){
    float dist = ((*sensor_pointer) / CLOCK_TO_NANO) / NANO_TO_CM;
    return dist;
}


void killWhile(){
    stopWhile = 0;
}


//creates a queue of the last X number of ultrasonic distance sensor outputs
//to allow for later filtering of data
void weighted_distance_sensor(volatile int *sideways_sensor, volatile int *backwards_sensor){
    queue1 = createQueue(queueNum);
    queue2 = createQueue(queueNum);

    while(stopWhile){

        nanosleep((const struct timespec[]){{0, 60000000L}}, NULL); //delay of 60 millisecond
        if(isFull(queue1)){
            dequeue(queue1);
        }
        if(isFull(queue2)){
            dequeue(queue2);
        }
        enqueue(queue1, read_distance_sensor(sideways_sensor));
        enqueue(queue2, read_distance_sensor(backwards_sensor));
    }
}


//utilizes mean filtering technique to take average of the distance values in the queue
float query_weighted_distances(int sensor){
    if(sensor == 1){
        float sum;
        int i = 0;
        for(i = 0; i < queueNum; i ++){
            sum = sum + queue1->array[i];
        }
        return sum/queueNum;
    }
    else if(sensor == 2){
        float sum;
        int i = 0;
        for(i = 0; i < queueNum; i ++){
            sum = sum + queue2->array[i];
        }
        return sum/queueNum;
    }
    return 0;
}


//returns servo position in degrees
float read_servo_direct(volatile int *encoder_pointer){
    int unitsFC = 360;
    float high_micro = (*encoder_pointer) / 100;
    float duty_cycle = 100 * (high_micro / ENCODER_PERIOD);
    float theta = ((duty_cycle - DUTY_CYCLE_MIN) * CIRCLE_UNITS) / (DUTY_CYCLE_MAX - DUTY_CYCLE_MIN + 1);
    return theta;
}


//get the median of a specified array, used for median filtering
float get_median(float *array, int size) {
    int i=0 , j=0 , temp=0;
    for(i=0 ; i<size ; i++)
    {
        for(j=0 ; j<size-1 ; j++)
        {
            if(array[j]>array[j+1])
            {
                temp        = array[j];
                array[j]    = array[j+1];
                array[j+1]  = temp;
            }
        }
    }
    int middle = size / 2;
    return array[middle];
}


//creates a queue data structure for the servo encoder feedback values
void weighted_encoder_fb(volatile int *left_servo_encoder, volatile int *right_servo_encoder){
    queue3 = createQueue(encoderQueueSize);
    queue4 = createQueue(encoderQueueSize);
    while(stopWhile){
        nanosleep((const struct timespec[]){{0, 3000000L}}, NULL); //delay of 3 millisecond
        if(isFull(queue3)){
            dequeue(queue3);
        }
        if(isFull(queue4)){
            dequeue(queue4);
        }
        enqueue(queue3, read_servo_direct(left_servo_encoder));
        enqueue(queue4, read_servo_direct(right_servo_encoder));
    }
}


//uses a median filtering technique to return the median of the wheel encoder feedback queue
float query_weighted_encoder(int wheel){
    float queueArr1[encoderQueueSize];
    float queueArr2[encoderQueueSize];
    if(wheel == 1){  //left side is wheel 1
        int i = 0;
        for(i = 0; i < encoderQueueSize; i ++){
            queueArr1[i] = queue3->array[i];
        }
        return get_median(queueArr1, encoderQueueSize);
    }
    else if(wheel == 2){  //right side is wheel 2
        int i = 0;
        for(i = 0; i < encoderQueueSize; i ++){
            queueArr2[i] = queue4->array[i];
        }
        return get_median(queueArr2, encoderQueueSize);
    }
    return 0;
}

