#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "process.h"
#include "messages/builder.h"

void process(int client, struct sockaddr_in* clientAddr)
{
	char* request = malloc(sizeof(char*));
	struct message* msg = malloc(sizeof(struct message));
	size_t termlen = strlen(TERMINATION);
	while (1) {
		if (get_request(request, client) == -1) {
			perror("Error while receiving message.");
			break;
		}
		if (message_from_string(msg, request) == -1) {
			perror("Error when parsing request.");
			break;
		}
		if (handle_request(msg) == -1) {
			perror("Error when handling request.");
			break;
		}
	}
	free(msg);
	free(request);
}


int handle_request(struct message* msg)
{
	switch (msg->type) {
		case 'C':
			printf("Proccessing command with code: %d (%s).\n", msg->code, msg->text);
			return 0;
		break;
		case 'I':
			printf("Proccessing command with code: %d (%s).\n", msg->code, msg->text);
			return 0;
		break;
		default:
			perror("Invalid message type.");
			return -1;
	}
}


int get_request(char* request, int client)
{
	char buffer[256];
	int length, bsize = sizeof(buffer);
	length = read(client, buffer, bsize);
	if (length == 0) {
		request = NULL;
		perror("Client closed connection.");
		return -1;
	}
	if (length == -1) {
		request = NULL;
		perror("Could not read from client.");
		return -1;
	}
	do {
		buffer[length] = '\0';
	} while (isspace(buffer[--length]));
	strncpy(request, buffer, strlen(buffer));
	return 0;
}
