#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <signal.h>

#include "barber.h"
#include "server.h"

#include "tools/procargs.h"
#include "protocol/server.h"
#include "semaphores/sems.h"
#include "shmemory/shared.h"
#include "messages/builder.h"

#define DEFAULT_PORT 4242

void cut_hair(int time)
{
	printf("Barber: Cutting hair for %d seconds.\n", time);
	sleep(time);
	printf("Barber: Hair cutted.\n");
}

int main(const int argc, const char* argv[])
{
	struct server s;
	s.port = DEFAULT_PORT;

	// Process user arguments.
	process_args(&s, argv, argc);

#ifdef DEBUG
	fprintf(stderr, "Server: Creating barber process.\n");
#endif

	int pid = fork();
	if (pid == -1) {
		perror("Error while creating barber process");
		return EXIT_FAILURE;
	}

	if (!pid) { // Barber (child) process.
		// Set shared memory and semaphores.
		struct shared* data = get_shared(getuid());
		data->semaphores = semaphores_init(getuid());
		if (data->semaphores == -1) {
			perror("Semaphores initialization error");
			return EXIT_FAILURE;
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
	}
	else {
		// Ignore SIGCHLD. 
		signal(SIGCHLD, SIG_IGN);

		// Initialize server socket and bind it.
		if (server_init(&s, s.port) == -1) {
			perror("Error while init. of server");
			return EXIT_FAILURE;
		}

		printf("Server is running on port: %d\n", s.port);
		server_start(&s); // Run Forest, run!

		// Stop server, free memory and wait.
		server_stop(&s);
	}

	// Try removing semaphores.
	int semaphores = semaphores_init(getuid());
	semctl(semaphores, 0, IPC_RMID, NULL);

	return EXIT_SUCCESS;
}
