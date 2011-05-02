#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "server.h"
#include "help.h"

#include "messages/builder.h"


#define DEFAULT_PORT 4242


// Semaphores definitions.
#define SEM_MUTEX     0
#define SEM_BARBER    1
#define SEM_CUSTOMERS 2

// Number of semaphores.
#define SEM_COUNT     3


int global_custommers = 0;


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

int main(const int argc, const char* argv[])
{
	process_args(argv, argc);

#ifdef DEBUG
	fprintf(stderr, "Creating barber process.\n");
#endif

	int pid = fork();
	if (pid == -1) {
		perror("Error while creating barber process.");
		return EXIT_FAILURE;
	}

	if (!pid) { // Barber (child) process.
		// Get existing semaphores.
		int semaphores = semget(getuid(), SEM_COUNT, 0666);
		if (semaphores == -1) {
			semaphores = semget(getuid(), SEM_COUNT, 0666 | IPC_CREAT);
			if (semaphores == -1) {
				perror("Semaphores cannot be created.");
				return EXIT_FAILURE;
			}
			
			// Set up  mutual exclusion.
			if (semctl(semaphores, SEM_MUTEX, SETVAL, 1) == -1) {
				perror("Mutex cannot be setted up.");
				return EXIT_FAILURE;
			}

			// Set up customers.
			if (semctl(semaphores, SEM_CUSTOMERS, SETVAL, 0) == -1) {
				perror("Customers cannot be setted up.");
				return EXIT_FAILURE;
			}

			// Set up barber.
			if (semctl(semaphores, SEM_BARBER, SETVAL, 0) == -1) {
				perror("Barber cannot be setted up.");
				return EXIT_FAILURE;
			}
		}
		printf("I am the barber.\n");
	}
	else {
		struct server* s = (struct server*) malloc(sizeof(struct server)); 
		if (server_init(s, global_port) == -1) {
			free(s);
			return EXIT_FAILURE;
		}

		printf("Server is running on port: %d\n", global_port);
		server_start(s);

		server_stop(s);
		free(s);
		wait();
	}
	return EXIT_SUCCESS;
}
