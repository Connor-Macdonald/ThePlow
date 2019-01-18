#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "address_map_arm.h"
#include "physical_address_access.h"
#include "servo_drive_functions.h"
#include "sensor_functions.h"
// TODO: Add to address map
#define RIGHT_SERVO 0x00000010
#define LEFT_SERVO 0x00000010


int main(void)
{
    // Initialization of FPGA bridge
    int fd = -1; // used to open /dev/mem
    void *LW_virtual; // physical addresses for light-weight bridge
    // Create virtual memory access to the FPGA light-weight bridge
    if ((fd = open_physical (fd)) == -1)
    return (-1);
    if (!(LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)))
    return (-1);

    volatile int * right_servo = (int *) (LW_virtual + RIGHT_SERVO); // virtual address pointer to right servo
    volatile int * left_servo; (int *) (LW_virtual + RIGHT_SERVO); // virtual address pointer to right servo
    
    // Both servos max speed forward
    write_servo(4, right_servo);
    write_servo(4, left_servo);

    // Unmap FPGA bridge
    unmap_physical (LW_virtual, LW_BRIDGE_SPAN);
    close_physical (fd);
    return 0;
}





