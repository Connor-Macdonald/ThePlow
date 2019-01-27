#ifndef servo_drive_func
#define servo_drive_func

#define SPEED_HIGH 4
#define SPEED_LOW -4

#define DUTY_CYCLE_MIN 29
#define DUTY_CYCLE_MAX 971
#define CIRCLE_UNITS 360



/* 
write_servo
PARAMETER:
int speed: new speed value
volatile int servo_pointer: pointer to servo address
OUTPUT: 1 succes 0 failiure
Writes the new speed value to the servo
*/
int write_servo (int, volatile int);

/*
read_servo_pos
PARAMETER:
volatile int encoder_pointer: pointer to servo encoder address
OUTPUT: angle of servo at time requested
Reads the duty cycle of the encoder and calculates its angle 
*/
int read_servo_pos (volatile int );

#endif