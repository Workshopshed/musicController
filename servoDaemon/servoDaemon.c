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

#define no_servos 8
int const frequency_hz = 50; // default 50hz frequency to send pulses
// normal range is from 900 to 2100 for 120 degree servos
int const servo_range = 1200;
// servo center at 1500us
int const servo_mid = 1500;
// Servo pulse widths
//These pulses normally range from 900us to 2100us which usually corresponds to +- 60 degrees of rotation from the neutral position.
//1500us usually corresponds to the center position.

pthread_t servoThread;
pthread_mutex_t lock;
volatile sig_atomic_t done = 0;

struct listener_struct {
    pthread_t tid;
    int pipeHandle;
    int servo_pulse_width_us;
};

struct listener_struct listener_data[no_servos + 1];

//Max message for pipes
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

    rc_disable_signal_handler(); //Implement our own signal handler so we can shut down the threads correctly
    signal(SIGTERM, term);
    signal(SIGINT, term);

    int err;
    int i;

    for (i = 1; i <= no_servos; i++) {

        createServoPipes(i);
        listener_data[i].pipeHandle = openPipe(i); 

	// http://stackoverflow.com/questions/19232957/pthread-create-passing-an-integer-as-the-last-argument
        int *arg = malloc(sizeof(*arg));
        if (arg == NULL) {
            fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
            exit(EXIT_FAILURE);
        }
        *arg = i;

        err = pthread_create(&(listener_data[i].tid), NULL, servoListener, arg);
        if (err != 0)
            printf("Can't create listener thread :[%s]\n", strerror(err));
    }

    err = pthread_create(&(servoThread), NULL, servoRunner, NULL);
    if (err != 0)
        printf("Can't create runner thread :[%s]\n", strerror(err));

    //Wait till done, pthread_join blocks the signals
    while (!done) {
        rc_usleep(1000);
    }
    //Should I be doing a pthread_join here?

    //Cleanup
    for (i = 1; i <= no_servos; i++) {
        close(listener_data[i].pipeHandle);
        rollbackPipes(i);
    }

    pthread_mutex_destroy(&lock);

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
    char servoChildDir[255];
    char servoPosition[255];

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

int openPipe(int channel) {
    char servoPosition[255];

    sprintf(servoPosition, "%s/servo%d/position", servoDir, channel);
    printf("Listening on %s\n", servoPosition);

    return open(servoPosition, O_RDONLY | O_NONBLOCK);  
}

void *servoListener(void *i) {
    int channel = *((int *) i);
    free(i);

    char rd_buffer[BUF_SIZE];
    
    memset(rd_buffer, 0, sizeof(rd_buffer));  //Clear buffer

    while(!done) {
        if (read(listener_data[channel].pipeHandle, rd_buffer, sizeof(rd_buffer)) > 0)
		processCommand(channel, rd_buffer);
    }
    printf("Shutting down listener\n");
    
    return;

}

void processCommand(int channel, char *buffer) {
    double servo_pos = 0;

    printf("Received: %s\n", buffer);

    servo_pos = atof(buffer);
    if (servo_pos <= 1.5 && servo_pos >= -1.5) {
        printf("Moving servo %d to %f\n",channel,servo_pos);

        pthread_mutex_lock(&lock);

            listener_data[channel].servo_pulse_width_us = servo_mid + (servo_pos*(servo_range/2));

        pthread_mutex_unlock(&lock);
    }
    return;
}

void* servoRunner(void *a) {

    int toggle = 0;
    int pulse_width[no_servos+1]; //Local copy of pulses
    
    int i;

    while (!done) {

        //Get updated values
        if (pthread_mutex_trylock(&lock) == 0)
        {
            for (i = 1; i <= no_servos; i++) {
                pulse_width[i] = listener_data[i].servo_pulse_width_us;
            }
            pthread_mutex_unlock (&lock);
        }

        //Move servos
        for (i = 1; i <= no_servos; i++) {
            if (pulse_width[i] != 0) {
                rc_send_servo_pulse_us(i, pulse_width[i]);
            }
        }

        // blink green led
        rc_set_led(GREEN, toggle);
        toggle = !toggle;

        // sleep roughly enough to maintain frequency_hz
        rc_usleep(1000000 / frequency_hz);
    }
    printf("Shutting down runner\n");
    return;
}