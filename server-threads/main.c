#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools/procargs.h"
#include "protocol/server.h"

#define DEFAULT_PORT 8080

int main(const int argc, const char* argv[])
{
	// Define server structure
	// and set default port.
	struct server s;
	s.port = DEFAULT_PORT;

	// Process program arguments.
	process_args(&s, argv, argc);

	// Initialize server socket and bind it.
	if (server_init(&s, s.port) == -1) {
		perror("Error while init. of server");
		return EXIT_FAILURE;
	}

	server_start(&s);

	// Stop server.
	server_stop(&s);

	return EXIT_SUCCESS;
}
