#ifndef servo_drive_func
#define servo_drive_func

#define SPEED_HIGH 16
#define SPEED_LOW -16

#define ENCODER_PERIOD 1099
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

// forward_nseconds(time forwards, left servo pointer, right servo pointer)
	// left_motor_forward_nseconds
	// right motor_forward_nseconds
void forward_nseconds(int, volatile int, volatile int);

// reverse_nseconds(time backwards, left servo pointer, right servo pointer)
	// right_motor_reverse_nseconds
	// left_motor_reverse_nseconds
void reverse_nseconds(int, volatile int, volatile int);

// turn_n_degrees
	// if degree > 0 turn left that much
	// if degree < 0 turn right that much
void turn_n_degrees(int, volatile int, volatile int);


#endif