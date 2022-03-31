/*********************************************\
* pl32lib v0.05                               *
* (c)2022 pocketlinux32, Under Lesser GPLv2.1 *
* Serial Communication module                 *
\*********************************************/
#pragma once
#include <pl32-memory.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

typedef struct plterminal {
	int fDesc;
	struct termios termOptions;
};

int plOpenTerminal(char path);
void plTermSend(char* string);
char* plTermGet(int bytes);
