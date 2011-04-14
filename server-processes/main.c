#include <stdio.h>
#include <stdlib.h>
#include "server.h"

int main (const int argc, const char* argv[])
{
	struct server* s = malloc(sizeof(struct server)); 
	if (server_init(s, 4243) == -1) {
		free(s);
		return EXIT_FAILURE;
	}
	// start loop with accept
	server_stop(s);
	free(s);
	return EXIT_SUCCESS;
}
