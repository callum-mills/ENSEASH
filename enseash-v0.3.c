#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define MOTD "Welcome to ENSEASH !\n"
#define PROMPT "ESH >> "

// Magic Numbers :
#define MAX_INPUT_LEN 1024
#define OUTPUT_BUFFER_SIZE 1024
#define STDOUT 1
#define STDIN 0
#define LINE_FEED 10
#define END_OF_TRANSMISSION 4


int readline(char * buffer, int buffer_size, int fd){ // Wrapper for read() which reads to a buffer until line end

	int i;
	char c; //Single character buffer

	i = 0;
	c = 0;

    if(buffer_size < 1){return EXIT_FAILURE;}

    read(fd, &c, 1);
    if(c == 0){exit(0);} //exit if <Ctrl+D> in empty prompt

    if(c != LINE_FEED) {

			if (buffer_size < 1) {return EXIT_FAILURE;}
			buffer[i] = c;

		    } else {
                buffer[i] = 0;
                return EXIT_SUCCESS;
            }

	while (1) {

		if (read(fd, &c, 1) != 0){ //If there is something to read
            i++;

            if(c != LINE_FEED) {

			if (i > buffer_size) {return EXIT_FAILURE;}
			buffer[i] = c;

		    } else {
                buffer[i] = 0;
                return EXIT_SUCCESS;
            }
	    }   
    }
}

int main(){

    char inbuff[MAX_INPUT_LEN];

    int pid, status;

    write(STDOUT, MOTD, 22); // message displayed at shell start defined in MOTD

    while (1) {

        write(STDOUT, PROMPT, 8);
        readline(inbuff, MAX_INPUT_LEN, STDIN);

        if (!(strcmp(inbuff, "exit"))) {
            return EXIT_SUCCESS;
        } else {

            pid = fork(); // the fork allows the shell (parent process) to stay alive while executing selected command

            if (pid != 0){ // pid == 0 in child process only
			wait(&status);
                wait(&status);
            } else {execlp(inbuff, inbuff, (char*)NULL); exit(0);}
        }
    }
}
