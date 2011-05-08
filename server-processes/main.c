#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <signal.h>

#include "barber/barber.h"
#include "tools/procargs.h"
#include "protocol/server.h"
#include "semaphores/sems.h"
#include "shmemory/shared.h"
#include "messages/builder.h"

#define DEFAULT_PORT 4242


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
		barber();
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
