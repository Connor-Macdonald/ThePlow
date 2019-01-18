#include "physical_address_access.h"

int write_servo (int speed, volatile int servo_pointer){
    if( speed > SPEED_LOW && speed < SPEED_HIGH){
        // Success write speed
        *servo_pointer = speed
        return 1
    }
    // failiure 
    else return 0
}