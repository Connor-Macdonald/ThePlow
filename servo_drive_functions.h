#ifndef servo_drive_func
#define servo_drive_func

#define SPEED_HIGH 100
#define SPEED_LOW -100

#define ENCODER_PERIOD 1099
#define DUTY_CYCLE_MIN 2.9
#define DUTY_CYCLE_MAX 97.1
#define CIRCLE_UNITS 360

//chane this
#define OUTLIER_THRESHOLD 100

/*
write_servo
PARAMETER:
int speed: new speed value
volatile int servo_pointer: pointer to servo address
if left: write 1 for left servo, 0 for right
OUTPUT: 1 success 0 failure
Writes the new speed value to the servo
*/
int write_servo (int, volatile int *, int);

/* 
write_servo_direct
PARAMETER:
int speed: new speed value
volatile int servo_pointer: pointer to servo address
if left:
OUTPUT: 1 success 0 failure
Writes the new speed value to the servo
*/
int write_servo_direct (int, volatile int *); //writes to the servo

/*
read_servo_pos
PARAMETER:
volatile int encoder_pointer: pointer to servo encoder address
OUTPUT: angle of servo at time requested
Reads the duty cycle of the encoder and calculates its angle 
*/
float read_servo_pos (volatile int *);

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
