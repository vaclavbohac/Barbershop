#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "client.h"
#include "messages/builder.h"
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


void send_time(struct client* cli)
{
	struct message request;

	// Send custommer haircut length.
	message_init(&request, ANSWER, 8, "seconds");
#ifdef DEBUG
	printf("%c%d:%s\n", request.type, request.code, request.text);
#endif
	if (send_request(cli, &request) == -1) {
		fprintf(stderr, "Sending message to server failed.\n");
		return;
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
	printf("Sending request %c:%s\n", request.type, request.text);
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
		printf("Chair message: %s\n", response.text);
		if (get_response(cli, &response) == -1) {
			fprintf(stderr, "Error while waiting for 'sit'.\n");
			return -1;
		}

		if (strcmp(response.text, "sit") != 0) {
			fprintf(stderr, "Got different message than sit.\n");
			return -1;
		}

		// Send information about time.
		send_time(cli);

		if (get_response(cli, &response) == -1) {
			fprintf(stderr, "Error while waiting for 'done'.\n");
			return -1;
		}

		if (strcmp(response.text, "done") != 0) {
			fprintf(stderr, "Got different message than done.\n");
			return -1;
		}

		message_init(&response, ANSWER, 0, "bye");
		if (send_request(cli, &request) == -1) {
			fprintf(stderr, "Error while sending request.\n");
			return -1;
		}
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
