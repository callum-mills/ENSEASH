#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MOTD "Welcome to ENSEASH !\n"

int main(){
	write(1,MOTD,24); // message displayed at shell start defined in MOTD
	return EXIT_SUCCESS;
}