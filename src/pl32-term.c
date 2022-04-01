/*********************************************\
* pl32lib, v0.06                              *
* (c)2022 pocketlinux32, Under Lesser GPLv2.1 *
* Serial Communication module                 *
\*********************************************/
#include <pl32-term.h>

// Opens a terminal session
plterminal_t* plOpenTerminal(char* path, plgc_t* gc){
	plterminal_t* returnTerminal = NULL;

	int fileDesc = open(path, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if(fileDesc == -1)
		perror("plOpenTerminal");

	if(fileDesc > 0){
		returnTerminal = plGCAlloc(gc, sizeof(plterminal_t));
		returnTerminal->termOptions = plGCAlloc(gc, sizeof(struct termios));

		returnTerminal->fDesc = fileDesc;
	}

	return returnTerminal;
}

// Closes a terminal session
void plCloseTerminal(plterminal_t* terminalSession, plgc_t* gc){
	close(terminalSession->fDesc);
	plGCFree(gc, terminalSession->termOptions);
	plGCFree(gc, terminalSession);
}

// Initializes the terminal session with settings similar to BSD raw mode
// It's not a 100% match to BSD raw mode
void plTermRawInit(plterminal_t* terminalSession){
	cfmakeraw(terminalSession->termOptions);
	cfsetispeed(terminalSession->termOptions, B9600);
	cfsetospeed(terminalSession->termOptions, B9600);

	terminalSession->termOptions->c_cflag |= (CLOCAL | CREAD);
	terminalSession->termOptions->c_cc[VMIN] = 1;
	terminalSession->termOptions->c_cc[VTIME] = 0;

	tcsetattr(terminalSession->fDesc, TCSANOW, terminalSession->termOptions);
}

// Sends byte array to the terminal session
void plTermSend(plterminal_t* terminalSession, plarray_t* string){
	write(terminalSession->fDesc, string->array, string->size);
}

// Sends one byte to the terminal session
void plTermSendC(plterminal_t* terminalSession, char c){
	write(terminalSession->fDesc, &c, 1);
}

// Gets byte array from the terminal session
plarray_t* plTermGet(plterminal_t* terminalSession, plgc_t* gc){
	plarray_t* returnString = plGCAlloc(gc, sizeof(plarray_t));
	char readChar;

	returnString->array = plGCAlloc(gc, 2 * sizeof(char));
	returnString->size = 0;

	while(read(terminalSession->fDesc, &readChar, 1) > 0){
		if(returnString->size > 1){
			void* tempVar = plGCAlloc(gc, (returnString->size + 1) * sizeof(char));

			if(!tempVar){
				plGCFree(gc, returnString->array);
				plGCFree(gc, returnString);
				return NULL;
			}

			returnString->array = tempVar;
		}

		((char*)returnString->array)[returnString->size] = readChar;
		returnString->size++;
	}
}

// Get one byte from the terminal session
char plTermGetC(plterminal_t* terminalSession){
	char c;
	if(read(terminalSession->fDesc, &c, 1) > 0)
		return c;

	return 0;
}

// Sets up terminal for an interactive session, basically an oversimplified picocom
void plTermInteractive(plterminal_t* terminalSession){
	struct termios stdio;
	int fcstdio = fcntl(STDIN_FILENO, F_GETFL);
	char iochar = 0;
	tcgetattr(STDIN_FILENO, &stdio);

	tcsetattr(terminalSession->fDesc, TCSANOW, terminalSession->termOptions);
	tcsetattr(STDOUT_FILENO, TCSANOW, terminalSession->termOptions);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, terminalSession->termOptions);
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

	while(iochar != 1){
		while(read(terminalSession->fDesc, &iochar, 1) > 0)
			write(STDOUT_FILENO, &iochar, 1);

		while(read(STDIN_FILENO, &iochar, 1) > 0)
			write(terminalSession->fDesc, &iochar, 1);
	}

	tcsetattr(STDIN_FILENO, 0, &stdio);
	tcsetattr(STDOUT_FILENO, 0, &stdio);
	fcntl(STDIN_FILENO, F_SETFL, fcstdio);
}
