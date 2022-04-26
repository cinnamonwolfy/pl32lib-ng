/*********************************************\
* pl32lib, v1.03                              *
* (c)2022 pocketlinux32, Under Lesser GPLv2.1 *
* Serial Communication module                 *
\*********************************************/
#pragma once
#include <pl32-memory.h>
#include <pl32-shell.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

typedef struct plterminal {
	int fDesc;
	struct termios* termOptions;
} plterminal_t;

plterminal_t* plOpenTerminal(char* path, plgc_t* gc);
void plCloseTerminal(plterminal_t*, plgc_t* gc);
void plTermRawInit(plterminal_t* terminalSession);

void plTermSend(plterminal_t* terminalSession, plarray_t* string);
void plTermSendC(plterminal_t* terminalSession, char c);
plarray_t* plTermGet(plterminal_t* terminalSession, plgc_t* gc);
char plTermGetC(plterminal_t* terminalSession);

void plTermInteractive(plterminal_t* terminalSession);
