//
// Created by Andy.Clark on 17/03/2017.
//
#include <stdio.h>
#include "rc_usefulincludes.h"
#include "roboticscape.h"

int main() {
    printf("RC Test!\n");

    rc_initialize();
    rc_send_servo_pulse_normalized_all(0);
    rc_enable_servo_power_rail();
    rc_usleep(500000);

    rc_disable_servo_power_rail();

    rc_cleanup();

    return 0;
}

