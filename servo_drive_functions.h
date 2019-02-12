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
int write_servo (int, volatile int *);

/*
read_servo_pos
PARAMETER:
volatile int encoder_pointer: pointer to servo encoder address
OUTPUT: angle of servo at time requested
Reads the duty cycle of the encoder and calculates its angle 
*/
int read_servo_pos (volatile int );

/*
 * smart_turn_right
 * PARAMETER:
 * volatile int left_pointer: pointer to left servo motor
 * volatile int right_pointer: pointer to right servo motor
 */
void smart_turn_right (volatile int, volatile int);

void smart_turn_left (volatile int, volatile int);

void smart_turn (volatile int, volatile int);

void gyro_read_and_adjust(volatile int, volatile int);

void write_servo_angle(volatile int);

#endif
