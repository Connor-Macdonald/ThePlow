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

float weighted_distance_sensor(volatile int *, volatile int *){
    queue1 = createQueue(queueNum);
    queue2 = createQueue(queueNum);

    while(true){
        if(isFull(queue1)){
            dequeue(queue1)
        }
        if(isFull(queue2)){
            dequeue(queue2)
        }
        enqueue(queue1, read_distance_sensor())

    }
}