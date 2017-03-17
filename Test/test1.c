#include <stdio.h>
#include "rc_usefulincludes.h"
#include "roboticscape.h"

//Simple test for playing a note on the xylophone

int main() {
    printf("RC Test!\n");

    rc_initialize();
    rc_enable_servo_power_rail();

    printf("Up\n");
    rc_send_servo_pulse_normalized(1, -1);
    rc_usleep(1000000);
    printf("Down!\n");
    rc_send_servo_pulse_normalized(1, 0.2);
    rc_usleep(200000);
    printf("Up\n");
    rc_send_servo_pulse_normalized(1, -1);
    rc_usleep(1000000);

    rc_disable_servo_power_rail();
    rc_cleanup();

    return 0;
}