#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include "process.h"
#include "barber.h"
#include "semaphores/sems.h"
#include "shmemory/shared.h"
#include "protocol/request.h"
#include "protocol/response.h"

void get_haircut()
{
	printf("I am getting haircut\n");
}

void process(int handle)
{
#ifdef DEBUG
	printf("Creating client structure.\n");
#endif
	char buffer[256];
	struct message request, response, sit;

	struct client cli;
	cli.handle = handle;

	int uid = getuid();

	// Create semaphores and shared memory.
	struct shared* data = get_shared(uid);
	int semaphores = semaphores_init(uid);
	if (semaphores == -1) {
		fprintf(stderr, "Error while getting semaphores.\n");
		return;
	}

	// Get client request.
	if (get_request(&cli, &request) == -1) {
		fprintf(stderr, "Error while getting request.\n");
		return; // Stop communication.
	}

	// Is request something different than 'enter'?
	if (strcmp(request.text, "enter") != 0) {
		fprintf(stderr, "Bad protocol iniciation.\n");
		return;
	}
	printf("<= enter\n");

	// Enter critical section.
	down(semaphores, SEM_MUTEX);

	if (data->custommers < MAX_CHAIRS) {
		// Chair is empty.
		int len = sprintf(buffer, "chair %d is empty",
					data->custommers + 1);
		buffer[len] = '\0';
		message_init(&response, INFORMATION, 1,
						"chair is empty");
		if (send_response(&cli, &response) == -1) {
			fprintf(stderr, "Error chair free.\n");
			up(semaphores, SEM_MUTEX);
			return;
		}
		if (get_request(&cli, &request) == -1) {
			fprintf(stderr, "Error while handshaking.\n");
			up(semaphores, SEM_MUTEX);
			return;
		}
		if (request.type != ANSWER
			|| strcmp(request.text, "ok")) {
			fprintf(stderr, "Handshake error.\n");
			up(semaphores, SEM_MUTEX);
			return;
		}
		printf("=> chair is free\n");
	}
	else {
		// Chair is full.
		message_init(&response, INFORMATION, 0,
					"chairnotfree");
		if (send_response(&cli, &response) == -1) {
			fprintf(stderr, "Error chair not free.\n");
			up(semaphores, SEM_MUTEX);
			return;
		}
		printf("=> chair not free\n");
		up(semaphores, SEM_MUTEX);
		return;
	}

	// Add custommer.
	data->custommers += 1;
	up(semaphores, SEM_CUSTOMMERS);

	message_init(&sit, COMMAND, 0, "sit");
	if (send_response(&cli, &sit) == -1) {
		fprintf(stderr, "Error while sending response.\n");
		up(semaphores, SEM_MUTEX);
		return;
	}
	printf("=> sit\n");

	if (get_request(&cli, &request) == -1) {
		fprintf(stderr, "Error while getting request.\n");
		up(semaphores, SEM_MUTEX);
		return;
	}

	if (strcmp(request.text, "seconds") != 0) {
		fprintf(stderr, "Expecting seconds.\n");
		up(semaphores, SEM_MUTEX);
		return;
	}
	printf("<= seconds\n");

	data->times[data->custommers - 1] = request.code;

	// Leave critical section.
	up(semaphores, SEM_MUTEX);
	// Wait for barber. 
	down(semaphores, SEM_BARBER);

	get_haircut();

	// Randezvous paradigm.
	up(semaphores, SEM_SITTING);
	down(semaphores, SEM_CUTTED);

	message_init(&response, INFORMATION, 0, "done");
	if (send_response(&cli, &response) == -1) {
		fprintf(stderr,
			"Error while sending 'done' response.\n");
		return;
	}
	printf("=> done\n");

	if (get_request(&cli, &request) == -1) {
		fprintf(stderr,
			"Error while waiting for request.\n");
		return;
	}
	printf("<= bye\n");

	return;
}
