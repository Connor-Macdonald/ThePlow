#include "sensor_functions.h"


float read_distance_sensor(volatile int sensor_pointer){
    float dist = (*sensor_pointer / CLOCK_TO_NANO) / NANO_TO_CM;
    return dist;
}