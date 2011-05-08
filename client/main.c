#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "help.h"
#include "client.h"

#define DEFAULT_HOST "localhost"
#define DEFAULT_PORT 8080
#define DEFAULT_TIME 10

int main(int argc, const char** argv)
{
	struct client cli;

	// Set default hostname.
	strcpy(cli.host, DEFAULT_HOST);

	// Set def. port & time.
	cli.port = DEFAULT_PORT;
	cli.time = DEFAULT_TIME;

	// Process user arguments.
	process_args(&cli, argv, argc);

	printf("Connecting to %s:%d\n", cli.host, cli.port);
	if (client_init(&cli, cli.host, cli.port) == -1) {
		return EXIT_FAILURE;	
	}

	client_start(&cli);

	client_stop(&cli);
	return EXIT_SUCCESS;
}
