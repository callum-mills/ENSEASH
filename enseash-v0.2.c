#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MOTD "Welcome to ENSEASH !\n"
#define PROMPT "ESH >> "

// Magic Numbers :
#define MAX_INPUT_LEN 1024
#define STDOUT 1
#define STDIN 0

int readline(char * buffer, int buffer_size, int fd){ // Wrapper for read() which reads to a buffer until line end

    int i;
    char c; //One character buffer

    i = -1;
    c = 0;

    while (1) { //While not EOL

        if (read(fd, &c, 1) != 0){ //If there is something to read
            i++;
            if(c != 10) {

            if (i > buffer_size) {return -1;}
            buffer[i] = c;

            } else {
                buffer[i] = 0;
                return 1;
            }
        }   
    }
}

int main(){

    char inbuff[MAX_INPUT_LEN];

    int pid, status;

    write(STDOUT, MOTD, 21); // message displayed at shell start defined in MOTD

    while (1) {

        write(STDOUT, PROMPT, 8);
        readline(inbuff, MAX_INPUT_LEN, STDIN);

		pid = fork();

		if (pid != 0){ // pid == 0 in child process only
			wait(&status);
		} else {execlp(inbuff, (char*)NULL); exit(0);}

    }
}