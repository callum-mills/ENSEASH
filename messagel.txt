#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#define MOTD "Welcome to ENSEASH !\n"
#define PROMPT "ESH >> "

// Magic Numbers :
#define MAX_INPUT_LEN 1024
#define OUTPUT_BUFFER_SIZE 1024
#define STDOUT 1
#define STDIN 0
#define LINE_FEED 10
#define END_OF_TRANSMISSION 4
#define SPACE 32
#define MAX_ARG_NBR 100


int readline(char* buffer, int buffer_size, int fd){

	int i;
    int k;
	char c; //One character buffer
    i = 0;
	c = 0;

    if(buffer_size < 1){return -1;}

    read(fd, &c, 1);
    if(c == 0){exit(0);}

    if(c != LINE_FEED) {

			if (buffer_size < 1) {return -1;}
			buffer[i] = c;

		    } else {
                buffer[i] = 0;
                return 1;
            }

	while (1) { //While not EOL

		if (read(fd, &c, 1) != 0){ //If there is something to read
            i++;

            if(c != LINE_FEED) {

			if (i > buffer_size) {return -1;}
			buffer[i] = c;

		    } else {
                buffer[i] = 0;
                return 1;
            }
	    }   
    }
}

int parse_arguments(char** command, char * buffer, int buffer_size){ //Return the number of arguments, including the command

    char arg[MAX_INPUT_LEN] = {0}; //An argument can't exceed the lengt of the input, so this is a safe value
    char c;
    char * stok;
    int nbr_args;

    nbr_args = 0;

    if(buffer_size < 1){return -1;}

    stok = strtok(buffer, " ");

    while(stok != NULL){
        command[nbr_args] = stok;
        stok = strtok((char*)NULL, " ");
        nbr_args++;
    }

    return nbr_args;

}

int main(){

    char inbuff[MAX_INPUT_LEN];

    int pid, status;

    write(STDOUT, MOTD, 22); // message displayed at shell start defined in MOTD

    while (1) {

        write(STDOUT, PROMPT, 8);
        readline(inbuff, MAX_INPUT_LEN, STDIN);
        
        if (!(strcmp(inbuff, "exit"))) {
            exit(0);
        } else {

            char* command[MAX_ARG_NBR];
            int nbr_args;
            nbr_args = parse_arguments(command, inbuff, MAX_INPUT_LEN);

            pid = fork();

            if (pid != 0){

                struct timespec tp;
                clock_gettime(_POSIX_MONOTONIC_CLOCK, &tp);
                long start_ms = tp.tv_nsec/100000;

                wait(&status);

                clock_gettime(_POSIX_MONOTONIC_CLOCK, &tp);
                long stop_ms = tp.tv_nsec/100000;

                char* output_buffer[OUTPUT_BUFFER_SIZE];

                if(snprintf(output_buffer, OUTPUT_BUFFER_SIZE,"exit:%d|%ldms\n", status, stop_ms-start_ms) > OUTPUT_BUFFER_SIZE){
                    write(STDOUT, "error buffer overflow", 20);
                }
                else {
                    sprintf(output_buffer, "exit:%d|%ldms\n", status, stop_ms-start_ms);
                }


                write(STDOUT, output_buffer, OUTPUT_BUFFER_SIZE);


            } else {

                char* arguments[nbr_args+1];

                for(int i = 0; i<nbr_args;i++){
                    arguments[i] = command[i];
                }

                arguments[nbr_args] = (char*)NULL;

                write(STDOUT, arguments[0], 2);
                
                execvp(command[0], arguments); 

                
                
                exit(0);}
        }
    }
}