//
// Created by Andy.Clark on 17/03/2017.
//

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>  //For Malloc

#include "threadtest.h"

int main() {

    pthread_t tid[8];

    printf("threadtest\n");

    int err;
    int i;

    for (i = 0; i < 3; i++) {

        int *arg = malloc(sizeof(*arg));
        if (arg == NULL) {
            fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
            exit(EXIT_FAILURE);
        }

        *arg = i;

        err = pthread_create(&(tid[i]), NULL, servoListener, arg);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        else
            printf("\n Thread %d created successfully\n", i);

    }

    //Wait for threads
    for (i = 0; i < 3; i++) {
        pthread_join(tid[i], NULL);
    }


    return 0;
}


void* servoListener(void *i)
{
    printf("Running in thread");

    int channel = *((int *) i); // http://stackoverflow.com/questions/19232957/pthread-create-passing-an-integer-as-the-last-argument
    free(i);

    printf("Listening on channel %d\n",channel);

    return;

}