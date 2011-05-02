#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "process.h"
#include "barber.h"
#include "tools/tools.h"
#include "messages/builder.h"
#include "shmemory/shared.h"

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
		if (handle_request(client, msg) == -1) {
			fprintf(stderr, "Error when handling request.\n");
			break;
		}
	}
	free(request);
	free(msg);
}

int handle_request(int client, struct message* msg)
{
	char buf[256];
	switch (msg->type) {
		case COMMAND:
			printf("Processing command with code: %d (%s).\n", msg->code, msg->text);
			if (!strcmp(msg->text, "enter")) {
				struct shared* data = get_shared(getuid());
				if (data->custommers < MAX_CHAIRS) {
					sprintf(buf, "chair %d is empty", data->custommers + 1);
					send_response(client, build(INFORMATION, 1, buf));
				}
				else {
					send_response(client, build(INFORMATION, 0, "chairnotfree"));
				}
			}
			return 0;
		break;
		case INFORMATION:
			printf("Processing command with code: %d (%s).\n", msg->code, msg->text);
			return 0;
		break;
		default:
			fprintf(stderr, "Invalid message type.\n");
			return -1;
	}
}

int send_response(int client, char* message)
{
	char buffer[256];
	int len = sprintf(buffer, "%s", message);
	return write(client, buffer, len);
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
