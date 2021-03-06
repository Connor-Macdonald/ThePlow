#ifndef sensor_functions
#define sensor_functions

#define CLOCK_TO_NANO 100
#define NANO_TO_CM 58

#define ENCODER_PERIOD 1099
#define DUTY_CYCLE_MIN 2.9
#define DUTY_CYCLE_MAX 97.1
#define CIRCLE_UNITS 360

/*
read_distance_sensor
PARAMETER:
volatile int sensor_pointer: pointer to sensor output
OUTPUT: distance recorded in cm
Reads the pluse time of the sensor and calculates the distance recorded
*/
float read_distance_sensor(volatile int *);

/*
Create two queues for distance sensor values
PARAMETER
sensor1, sensor2
*/
void create_distance_sensor_queues(volatile int *, volatile int *);

/*
returns the weighted averadges 
PARAMETER
int = 1 : return sensor1 NOTE: sideways sensor
int = 2 : return sensor2 NOTE: backwards sensor

NOTE: assumes that queue is full
*/
float query_weighted_distances(int);

float query_weighted_encoder(int);

/* NOTE look at this
    assumes that the side sensor is sensor 1 in the create_distance_sensor_queues

    returns the angle of the cart 0 is straight
*/
int check_side_distance();

#endif