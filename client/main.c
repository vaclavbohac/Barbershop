#include <stdlib.h>
#include <stdio.h>

#include "client.h"

int main(int argc, char** agrv)
{
	struct client* cli = (struct client*) malloc(sizeof(struct client));
	if (client_init(cli, "localhost", 4242) == -1) {
		free(cli);
		return EXIT_FAILURE;	
	}

	client_start(cli);

	client_stop(cli);
	free(cli);

	return EXIT_SUCCESS;
}
