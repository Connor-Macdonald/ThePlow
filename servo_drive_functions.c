#include "physical_address_access.h"

// NOTE: left and right must be opposite
int write_servo (int speed, volatile int servo_pointer){
    if( speed > SPEED_LOW && speed < SPEED_HIGH){
        // Success write speed
        *servo_pointer = speed
        return 1
    }
    // failiure 
    else return 0
}

int read_servo_pos (volatile int encoder_pointer) {
    int high_micro = *encoder_pointer / 100;
    int duty_cycle = 100 * (high_micro / ENCODER_PERIOD)
    int theta = ((duty_cycle - DUTY_CYCLE_MIN) * CIRCLE_UNITS) / (DUTY_CYCLE_MAX - DUTY_CYCLE_MIN + 1); 
    if(theta < 0) theta = 0;
    else if(theta > (unitsFC - 1)) theta = unitsFC - 1;
    return theta
}

// pivot on wheel (1 front left, 2 front right, 3 back left, 4 back right)