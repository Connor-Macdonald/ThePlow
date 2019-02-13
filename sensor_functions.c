#include "sensor_functions.h"
#include "queue.c"

struct Queue* queue1;
struct Queue* queue2;
int queueNum = 10;

float read_distance_sensor(volatile int *sensor_pointer){
        printf("Sensor pointer %d\n", *sensor_pointer);
        float dist = ((*sensor_pointer) / CLOCK_TO_NANO) / NANO_TO_CM;
    return dist;
}

void weighted_distance_sensor(volatile int *left_sensor, volatile int *right_sensor){
    queue1 = createQueue(queueNum);
    queue2 = createQueue(queueNum);

    while(true){
        if(isFull(queue1)){
            dequeue(queue1);
        }
        if(isFull(queue2)){
            dequeue(queue2);
        }
        enqueue(queue1, read_distance_sensor(left_sensor));
        enqueue(queue2, read_distance_sensor(right_sensor));
        sleep(1); // change to smaller values
    }
}

float query_weighted_distances(int sensor){
    if(sensor == 1){
        float sum;
        int i = 0;
        for(i = 0; i > queueNum; i ++){
            sum = sum + queue1->array[i];
        }
        return sum/queueNum;
    }
    else if(sensor == 2){
        float sum;
        int i = 0;
        for(i = 0; i > queueNum; i ++){
            sum = sum + queue2->array[i];
        }
        return sum/queueNum;
    }
}