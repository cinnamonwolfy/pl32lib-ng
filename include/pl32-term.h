/******************************************************************\
 pl32lib-ng, v0.95
 (c)2022 pocketlinux32, Under Lesser GPLv2.1
 pl32-term.h: Serial communication module header (platform-specific)
\*******************************************************************/
#pragma once
#include <pl32-memory.h>
#include <pl32-shell.h>

typedef struct plterminal {
	int fDesc;
	struct termios* termOptions;
} plterminal_t;

plterminal_t* plOpenTerminal(char* path, plmt_t* mt);
void plCloseTerminal(plterminal_t*, plmt_t* mt);
void plTermRawInit(plterminal_t* terminalSession);

ssize_t plTermSend(plterminal_t* terminalSession, plarray_t* string);
ssize_t plTermSendC(plterminal_t* terminalSession, char c);
plarray_t* plTermGet(plterminal_t* terminalSession, plmt_t* mt);
char plTermGetC(plterminal_t* terminalSession);

void plTermInteractive(plterminal_t* terminalSession);
