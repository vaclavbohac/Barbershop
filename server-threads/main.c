#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "barber/barber.h"
#include "tools/procargs.h"
#include "protocol/server.h"
#include "semaphores/sems.h"
#include "shmemory/shared.h"

#define DEFAULT_PORT 8080

void *run_barber(void *args)
{
	barber();
	pthread_exit((void *) 0);
}

int main(const int argc, const char* argv[])
{
	// Define server structure
	// and set default port.
	struct server s;
	s.port = DEFAULT_PORT;

	// Process program arguments.
	process_args(&s, argv, argc);

	pthread_t tid;
	pthread_create(&tid, NULL, run_barber, NULL);
	pthread_detach(tid);

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
