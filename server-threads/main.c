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

void *barber(void *args)
{
	// Set shared memory and semaphores.
	struct shared* data = get_shared(getuid());
	data->semaphores = semaphores_init(getuid());
	if (data->semaphores == -1) {
		perror("Semaphores initialization error");
		pthread_exit((void *) 1);
	}
	// Set customers to the default state.
	data->custommers = 0;
	while (1) {
#ifdef DEBUG
		printf("Barber: Custommers down.\n");
#endif
		// Wait for custommer.
		down(data->semaphores, SEM_CUSTOMMERS);
		// Enter critical section.
		down(data->semaphores, SEM_MUTEX);
		// Decrease number of custommers.
		data->custommers -= 1;
		// Get time from custommer.
		int time = data->times[data->custommers];
		// Release the kraken.
		up(data->semaphores, SEM_BARBER);
		// Leave critical section.
		up(data->semaphores, SEM_MUTEX);

		// Cut custommer's hair.
		cut_hair(time);

		// Run closing procedure.
		up(data->semaphores, SEM_CUTTED);
		down(data->semaphores, SEM_SITTING);
	}
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
	pthread_create(&tid, NULL, barber, NULL);
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
