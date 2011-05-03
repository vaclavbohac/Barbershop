#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "barber.h"
#include "server.h"
#include "help.h"

#include "protocol/server.h"
#include "semaphores/sems.h"
#include "shmemory/shared.h"
#include "messages/builder.h"

#define DEFAULT_PORT 4242

int global_port = DEFAULT_PORT;

void process_args(const char** args, const int length)
{
	char buf[128], *port = "--port=";
	int i, n = (int) strlen(port);
	for (i = 0; i < length; i++) {
		if (!strcmp(args[i], "--help")) {
			sprintf(buf, "%.*s", (int) strlen(args[0]) - 2, args[0] + 2);
			help(buf);
			exit(EXIT_SUCCESS);
		}
		if (!strncmp(args[i], port, n)) {
			sprintf(buf, "%.*s", 4, args[i] + n);
			global_port = atoi(buf);
		}
	}
}

void cut_hair(int time)
{
	printf("Cutting hair for %d seconds.\n", time);
	sleep(time);
	printf("Hair cutted.\n");
}

int main(const int argc, const char* argv[])
{
	// Process user arguments.
	process_args(argv, argc);

#ifdef DEBUG
	fprintf(stderr, "Creating barber process.\n");
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
			printf("Custommers down.\n");
#endif
			// Wait for custommer.
			down(data->semaphores, SEM_CUSTOMMERS);
			// Enter critical section.
			down(data->semaphores, SEM_MUTEX);
			// Decrease number of custommers.
			data->custommers -= 1;
			// Get time from custommer.
			int time = data->times[data->custommers];
			int handle = data->handles[data->custommers];
			// Release the kraken.
			up(data->semaphores, SEM_BARBER);
			// Leave critical section.
			up(data->semaphores, SEM_MUTEX);

			// Cut custommer's hair.
			cut_hair(time);
		}
	}
	else {
		struct server s;
		// Initialize server socket and bind it.
		if (server_init(&s, global_port) == -1) {
			perror("Error while init. of server");
			return EXIT_FAILURE;
		}

		printf("Server is running on port: %d\n", global_port);
		server_start(&s); // Run Forest, run!

		// Stop server, free memory and wait.
		server_stop(&s);
		wait();
	}

	// Try removing semaphores.
	int semaphores = semaphores_init(getuid());
	semctl(semaphores, 0, IPC_RMID, NULL);

	return EXIT_SUCCESS;
}
