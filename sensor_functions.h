#ifndef sensor_functions
#define sensor_functions

#define CLOCK_TO_NANO 0.02
#define NANO_TO_CM 58
/*
read_distance_sensor
PARAMETER:
volatile int sensor_pointer: pointer to sensor output
OUTPUT: distance recorded in cm
Reads the pluse time of the sensor and calculates the distance recorded
*/
float read_distance_sensor(volatile int);

#endif