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

	tcsetattr(terminalSession->fDesc, TCSANOW, terminalSession->terminalOptions);
}

void plTermSend(plterminal_t* terminalSession, char* string){
	write(terminalSession, string, strlen(string));
}

void plTermSendC(plterminal_t* terminalSession, char c){
	write(terminalSession, &c, 1);
}

plarray_t* plTermGet(plterminal_t* terminalSession, plgc_t* gc){
	plarray_t* returnString = plGCAlloc(gc, sizeof(plarray_t));
	char readChar;

	returnString->array = plGCAlloc(gc, 2 * sizeof(char));
	returnString->size = 0;

	while(read(terminalSession->fDesc, &readChar, 1) > 0){
		if(returnString->size > 1){
			void* tempVar = plGCAlloc(gc, (returnString->size + 1) * sizeof(char));

			if(!tempVar){
				plGCFree(returnString->array);
				plGCFree(returnString);
				return NULL;
			}

			returnString->array = tempVar;
		}

		((char*)returnString->array)[returnString->size] = readChar;
		returnString->size++;
	}
}

char plTermGetC(plterminal_t* terminalSesion){
	char c;
	if(read(terminalSession, &c, 1) > 0)
		return c;

	return 0;
}

void plTermInteractive(plterminal_t* terminalSession){
	struct termios stdio;
	char iochar;
	tcgetattr(STDIN_FILENO, &stdio);

	tcsetattr(terminalSession->fDesc, TCSANOW, terminalSession->terminalOptions);
	tcsetattr(STDOUT_FILENO, TCSANOW, terminalSession->terminalOptions);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, terminalSession->terminalOptions);
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCKING);

	while(iochar != 1){
		while(read(terminalSession->fDesc, &iochar, 1) > 0)
			write(STDOUT_FILENO, &iochar, 1);

		while(read(STDIN_FILENO, &iochar, 1) > 0)
			write(terminalSession->fDesc, &iochar, 1);
	}
}
