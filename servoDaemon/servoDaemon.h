//
// Created by Andy.Clark on 17/03/2017.
//

#include <stdbool.h>  //For bool

#ifndef SERVODAEMON_SERVODAEMON_H
#define SERVODAEMON_SERVODAEMON_H

void createServoDir();
void createServoPipes(int channel);
void* servoListener(void *i);
void* servoRunner(void *i);
void processCommand(int channel, char *buffer);
void rollbackPipes(int channel);
int openPipe(int channel);

#endif //SERVODAEMON_SERVODAEMON_H
