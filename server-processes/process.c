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

void process(int handle, struct sockaddr_in* clientAddr)
{
#ifdef DEBUG
	printf("Creating client structure.\n");
#endif
	char buffer[256];
	struct message request, response;

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

	// Enter critical section.
	down(semaphores, SEM_MUTEX);

	if (data->custommers < MAX_CHAIRS) {
		// Chair is empty.
		int len = sprintf(buffer, "chair %d is empty",
					data->custommers + 1);
		buffer[len] = '\0';
		message_init(&response, INFORMATION, 1, buffer);
		send_response(&cli, &response);
	}
	else {
		// Chair is full.
		message_init(&response, INFORMATION, 0,
					"chairnotfree");
		send_response(&cli, &response);
		up(semaphores, SEM_MUTEX);
		return;
	}

	// Add custommer.
	data->custommers += 1;
	up(semaphores, SEM_CUSTOMMERS);

	message_init(&response, COMMAND, 0, "sit");
	if (send_response(&cli, &response) == -1) {
		fprintf(stderr, "Error while sending response.\n");
		up(semaphores, SEM_MUTEX);
		return;
	}

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

	if (get_request(&cli, &request) == -1) {
		fprintf(stderr,
			"Error while waiting for request.\n");
		return;
	}
	printf("%s\n", request.text);

	return;
}

int handle_request(struct client* cli, struct message* request)
{
	char buf[256];
	int length, chair = 0;
	printf("Processing command with code: %d (%s).\n",
			request->code, request->text);
	switch (request->type) {
		case COMMAND:
			if (!strcmp(request->text, "enter")) {
				struct shared* data = get_shared(getuid());
				struct message response;
				if (data->custommers < MAX_CHAIRS) {
					length = sprintf(buf, "chair %d is empty", data->custommers + 1);
					buf[length] = '\0';
					message_init(&response, INFORMATION, 1, buf);
				}
				else {
					message_init(&response, INFORMATION, 0, "chairnotfree");
				}
				send_response(cli, &response);
			}
		break;
		case ANSWER:
			if (!strcmp(request->text, "chair")) {
				struct message time_req;

				chair = request->code;
				printf("Received chair: %d\n", chair);

				get_request(cli, &time_req);
				printf("%s\n", time_req.text);

				if (!strcmp(time_req.text, "seconds")) {
					struct shared* data = get_shared(getuid());	
					data->times[chair] = time_req.code;
					data->handles[chair] = cli->handle;
				}
				else {
					fprintf(stderr, "Wrong communication.\n");
					return -1;
				}
			}
		break;
		default:
			fprintf(stderr, "Invalid message type.\n");
			return -1;
	}
	return 0;
}
