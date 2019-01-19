#ifndef servo_drive_func
#define servo_drive_func

#define SPEED_HIGH 4
#define SPEED_LOW -4


/* 
PARAMETER:
int speed: new speed value
volatile int servo_pointer: pointer to servo address
OUTPUT: 1 succes 0 failiure
Writes the new speed value to the servo
*/

int write_servo (int, volatile int);
int test_func (int);

#endif