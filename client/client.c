#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "client.h"
#include "messages/builder.h"
#include "shmemory/shared.h"
#include "semaphores/sems.h"
#include "protocol/request.h"
#include "protocol/response.h"

int client_init(struct client* cli, char* host, int port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("Error while creating socket");
		return -1;
	}

	struct hostent *hostip = gethostbyname(host);
	if (!hostip) {
		perror("Error while getting address");
		return -1;
	}

	struct sockaddr_in clientAddr;
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(port);
	clientAddr.sin_addr = *(struct in_addr*) hostip->h_addr_list[0];

	if (connect(sock, (struct sockaddr*) &clientAddr, sizeof(clientAddr)) == -1) {
		perror("Cannot create connection with server");
		return -1;
	}

	cli->handle = sock;
	cli->port = port;

	return 0;
}

void send_time(struct client* cli, int chair)
{
	struct message request;

	// Send custommer chair.
	message_init(&request, ANSWER, chair, "chair");
	printf("%c%d:%s\n", request.type, request.code, request.text);
	if (send_request(cli, &request) == -1) {
		fprintf(stderr, "Sending message to server failed.\n");
		return;
	}

	// Send custommer haircut length.
	message_init(&request, ANSWER, 8, "seconds");
	printf("%c%d:%s\n", request.type, request.code, request.text);
	if (send_request(cli, &request) == -1) {
		fprintf(stderr, "Sending message to server failed.\n");
		return;
	}
}

void get_haircut(struct client* cli)
{
	printf("Getting haircut.\n");
	struct message request;
	message_init(&request, COMMAND, 0, "bye");
	if (send_request(cli, &request) == -1) {
		fprintf(stderr, "Error while closing connection.\n");
	}
}

int client_start(struct client* cli)
{
#ifdef DEBUG
	printf("Client is running.\n");
#endif
	struct message request;
	struct message response;

	message_init(&request, COMMAND, 0, "enter"); // 1. Send enter request.
#ifdef DEBUG
	printf("Got message %c:%s\n", request.type, request.text);
#endif
	if (send_request(cli, &request) == -1) {
		fprintf(stderr, "Error while sending request.\n");
		return -1;
	}

	if (get_response(cli, &response) == -1) { // 2. Get response.
		fprintf(stderr, "Error while getting response.\n");
		return -1;
	}

	printf("Message received: %s\n", response.text);
	if (!strcmp("chairnotfree", response.text)) {
		printf("Barbershop is full.\n");
	}
	else if(response.type == INFORMATION && response.code == 1) {
		// Chair is free.
		printf("%s\n", response.text);

		int uid = getuid();

		// Get shared memory and semaphores.
		struct shared* data = get_shared(uid);
		int semaphores = semaphores_init(uid);
		if (semaphores == -1) {
			fprintf(stderr, "Error while getting semaphores.\n");
			return -1;
		}
#ifdef DEBUG	
		printf("Mutex down.\n");
#endif
		// Enter critical section.
		down(semaphores, SEM_MUTEX);
		// Send information about time.
		send_time(cli, data->custommers);
		// Add custommer.
		data->custommers += 1;
		up(semaphores, SEM_CUSTOMMERS);
		// Leave critical section.
		up(semaphores, SEM_MUTEX);
		// Wait for barber. 
		down(semaphores, SEM_BARBER);

		get_haircut(cli);
	}
	else {
		printf("Unknown message.\n");
	}

	return 0;
}

void client_stop(struct client* cli)
{
	if ((void*)cli->handle != NULL) {
		close(cli->handle);	
	}
}
