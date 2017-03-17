//
// Created by Andy.Clark on 17/03/2017.
//
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
    rc_send_servo_pulse_normalized(2, 1);
    rc_send_servo_pulse_normalized(3, -1);
    rc_usleep(2000000);
    printf("Down!\n");
    rc_send_servo_pulse_normalized(1, 0.2);
    rc_send_servo_pulse_normalized(2, -0.2);
    rc_send_servo_pulse_normalized(3, 0.2);
    rc_usleep(2000000);
    printf("Up\n");
    rc_send_servo_pulse_normalized(1, -1);
    rc_send_servo_pulse_normalized(2, 1);
    rc_send_servo_pulse_normalized(3, -1);
    rc_usleep(5000000);

    rc_disable_servo_power_rail();

    rc_cleanup();

    return 0;
}
