#include <stdlib.h>
#include <stdio.h>

#include "client.h"

int main(int argc, char** agrv)
{
	struct client cli;
	if (client_init(&cli, "localhost", 4242) == -1) {
		return EXIT_FAILURE;	
	}
	client_start(&cli);
	client_stop(&cli);
	return EXIT_SUCCESS;
}
