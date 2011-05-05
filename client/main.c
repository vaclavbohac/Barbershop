#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "help.h"
#include "client.h"

#define DEFAULT_HOST "localhost"
#define DEFAULT_PORT 8080

int main(int argc, const char** argv)
{
	struct client cli;
	strcpy(cli.host, DEFAULT_HOST);
	cli.port = DEFAULT_PORT;

	process_args(&cli, argv, argc);
	printf("Connecting to %s:%d\n", cli.host, cli.port);

	if (client_init(&cli, cli.host, cli.port) == -1) {
		return EXIT_FAILURE;	
	}

	client_start(&cli);

	client_stop(&cli);
	return EXIT_SUCCESS;
}
