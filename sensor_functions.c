#include "sensor_functions.h"
//#include "queue.c"

int stopWhile = 1;

struct Queue* queue1;
struct Queue* queue2;
int queueNum = 15;
int old_side = 0;


float read_distance_sensor(volatile int *sensor_pointer){
        
        float dist = ((*sensor_pointer) / CLOCK_TO_NANO) / NANO_TO_CM;
    return dist;
}

void killWhile(){
    stopWhile = 0;
}

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

// Get distance
// int check_side_distance(){
//     float new_side = query_weighted_distances(1);
//     if(new_side < old_side){

//     }
//     else {
        
//     }
// }
