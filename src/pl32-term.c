/*********************************************\
* pl32lib v0.05                               *
* (c)2022 pocketlinux32, Under Lesser GPLv2.1 *
* Serial Communication module                 *
\*********************************************/
#include <pl32-term.h>

plterminal_t* plOpenTerminal(char* path, plgc_t* gc){
	plterminal_t* returnTerminal = NULL;

	int fileDesc = open(path, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if(fileDesc == -1)
		perror("plOpenTerminal");

	if(fileDesc > 0){
		returnTerminal = plGCAlloc(gc, sizeof(plterminal_t));
		returnTerminal->terminalOptions = plGCAlloc(gc, sizeof(struct termios));

		returnTerminal->fDesc = fileDesc;
	}

	return returnTerminal;
}

void plCloseTerminal(plterminal_t* terminalSession, plgc_t* gc){
	close(terminalSession->fDesc);
	plGCFree(terminalSession->terminalOptions);
	plGCFree(terminalSession);
}

void plTermRawInit(plterminal_t* terminalSession){
	cfmakeraw(terminalSession->terminalOptions);
	cfsetispeed(terminalSession->terminalOptions, B9600);
	cfsetospeed(terminalSession->terminalOptions, B9600);

	terminalSession->terminalOptions->c_cflags |= (CLOCAL | CREAD);
	terminalSession->terminalOptions->c_cc[VMIN] = 1;
	terminalSession->terminalOptions->c_cc[VTIME] = 0;
}

void plTermSend(plterminal_t* terminalSession, char* string){
	write(terminalSession, string, strlen(string));
}
