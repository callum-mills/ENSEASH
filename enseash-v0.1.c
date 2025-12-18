#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define STDOUT 1
#define STDIN 0

#define MOTD "Welcome to ENSEASH !\n"

int main(){
	write(STDOUT,MOTD,22); // message displayed at shell start defined in MOTD
	exit(EXIT_SUCCESS);
}