#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "process.h"
#include "tools.h"
#include "messages/builder.h"

void process(int client, struct sockaddr_in* clientAddr)
{
	char* request = (char*) malloc(sizeof(char*));
	struct message* msg = (struct message*) malloc(sizeof(struct message));
	while (1) {
		if (get_request(request, client) == -1) {
			fprintf(stderr, "Error while receiving message.\n");
			break;
		}
		rtrim(request);
		if (message_from_string(msg, request) == -1) {
			fprintf(stderr, "Error when parsing request.\n");
			break;
		}
		if (handle_request(msg) == -1) {
			fprintf(stderr, "Error when handling request.\n");
			break;
		}
	}
	free(request);
	free(msg);
}

int handle_request(struct message* msg)
{
	switch (msg->type) {
		case 'C':
			printf("Processing command with code: %d (%s).\n", msg->code, msg->text);
			return 0;
		break;
		case 'I':
			printf("Processing command with code: %d (%s).\n", msg->code, msg->text);
			return 0;
		break;
		default:
			fprintf(stderr, "Invalid message type.\n");
			return -1;
	}
}


int get_request(char* request, int client)
{
	char buffer[256];
	int length, bsize = sizeof(buffer);
	strcpy(request, ""); // Saves memory corruption.
	length = read(client, buffer, bsize);
	if (length == 0) {
		fprintf(stderr, "Client closed connection.\n");
		return -1;
	}
	if (length == -1) {
		fprintf(stderr, "Could not read from client.\n");
		return -1;
	}
	buffer[length] = '\0';
#ifdef DEBUG
	printf("Buffer in get_request: %s, length: %d\n", buffer, length);
#endif
	strncpy(request, buffer, length + 1);
	return 0;
}
