/* Created by Andy.Clark on 17/03/2017.

Initialise the RC code

For each servo create a folder
/var/servoDaemon/servo/servo1/
And some fifo pipes
/var/servoDaemon/power - to be implemented
/var/servoDaemon/servo0/position
        In response to commands on these pipes call the APIs
        If the term flag is raised then exit the thread

        Back on the main thread init sigterm listener and set a volatile flag if the data changes

        Wait for threads to join
*/

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <pthread.h>
#include <stdlib.h>  //For Malloc
#include <signal.h>

#include "rc_usefulincludes.h"
#include "roboticscape.h"

#include "servoDaemon.h"

static const char servoDir[] = "/var/servoDaemon";
pthread_t tid[9]; //Compensate for zero based indexes, tid[0] never used but we want the servo numbers to match the silk screen
int const no_servos = 8;
volatile sig_atomic_t done = 0;
volatile int toggle = 0;

#define BUF_SIZE 1024

void term(int signum) {
    printf("Signal!");
    done = 1;
}

int main() {

    printf("servoDaemon\n");
    printf("listens on named pipes %s/servo1 etc\n", servoDir);

    umask(0); //Set the process mask so that pipe permissions get created correctly

    createServoDir();

    rc_initialize();
    rc_enable_servo_power_rail();

    signal(SIGTERM, term);  //Need to put these after rc_init otherwise it will get reset
    signal(SIGINT, term);

    int err;
    int i;

    for (i = 1; i <= no_servos; i++) {

        createServoPipes(i);

        // http://stackoverflow.com/questions/19232957/pthread-create-passing-an-integer-as-the-last-argument
        int *arg = malloc(sizeof(*arg));
        if (arg == NULL) {
            fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
            exit(EXIT_FAILURE);
        }
        *arg = i;

        err = pthread_create(&(tid[i]), NULL, servoListener, arg);
        if (err != 0)
            printf("Can't create thread :[%s]\n", strerror(err));

    }

    //Wait till done
    while (!done) {
        rc_usleep(1000);
    }

    //Cleanup
    for (i = 1; i <= no_servos; i++) {
        rollbackPipes(i);
    }

    rc_disable_servo_power_rail();
    rc_cleanup();

    return 0;
}

void createServoDir() {
    DIR *pDir;
    pDir = opendir(servoDir);
    if (pDir == NULL) {
        printf("Create servo folder\n");
        int status;
        status = mkdir(servoDir, 0755);
    }
    return;
}

void createServoPipes(int channel) {
    char servoChildDir[20];
    char servoPosition[30];

    sprintf(servoChildDir, "%s/servo%d", servoDir, channel);
    sprintf(servoPosition, "%s/servo%d/position", servoDir, channel);

    DIR *pDir;
    pDir = opendir(servoChildDir);
    if (pDir == NULL) {
        printf("Create folder %s\n", servoChildDir);
        int status;
        status = mkdir(servoChildDir, 0755);
    }

    mkfifo(servoPosition, 0766);

}

void rollbackPipes(int channel) {
    char servoChildDir[20];
    char servoPosition[30];

    sprintf(servoChildDir, "%s/servo%d", servoDir, channel);
    sprintf(servoPosition, "%s/servo%d/position", servoDir, channel);

    DIR *pDir;
    pDir = opendir(servoChildDir);
    if (pDir != NULL) {
        unlink(servoPosition);
        rmdir(servoChildDir);
    }
}

void *servoListener(void *i) {
    int channel = *((int *) i);
    free(i);

    int fd = -1;

    char servoPosition[30];

    sprintf(servoPosition, "%s/servo%d/position", servoDir, channel);
    printf("Listening on %s\n", servoPosition);

    char rd_buffer[BUF_SIZE];

    fd = open(servoPosition, O_RDONLY);

    while (1) {
        memset(rd_buffer, 0, sizeof(rd_buffer));  //Clear buffer

        fd_set set;
        struct timeval timeout;
        int rv;

        // Read with timeout http://stackoverflow.com/questions/2917881/how-to-implement-a-timeout-in-read-function-call/
        FD_ZERO(&set); /* clear the set */
        FD_SET(fd, &set); /* add our file descriptor to the set */
        timeout.tv_sec = 0;
        timeout.tv_usec = 100;
        rv = select(fd + 1, &set, NULL, NULL, &timeout);
        if(rv == -1)
            perror("select"); /* an error accured */
        else if(rv == 0)
            printf("timeout"); /* a timeout occured */
        else {
            read(fd, rd_buffer, sizeof(rd_buffer));

            if (!processCommand(channel, rd_buffer)) {
                printf("Stopped listening on %d\n", channel);
                done = 1;
                break; //Exit while
            }
        }

        if (done) {
            printf("Terminating listener %d\n", channel);
            break;
        } //Main thread exited so we should too

        /* clean buf from any data */
        memset(rd_buffer, 0, sizeof(rd_buffer));

        rc_usleep(1000);
    }

    close(fd);

    return;

}

bool processCommand(int channel, char *buffer) {
    double servo_pos = 0;

    if ('Z' == buffer[0]) {
        return false;
    } else if (strcmp("", buffer) != 0) {
        printf("Received: %s\n", buffer);

        servo_pos = atof(buffer);
        if (servo_pos <= 1.5 && servo_pos >= -1.5) {
            printf("Moving servo %d to %f\n",channel,servo_pos);
            rc_send_servo_pulse_normalized(channel, servo_pos);
            // blink green led
            rc_set_led(GREEN, toggle);
            toggle = !toggle;

            // sleep a bit to maintain frequency_hz
            rc_usleep(1000);
        }
    }
    return true;
}
