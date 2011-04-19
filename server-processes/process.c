#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "process.h"
#include "messages/builder.h"

void process(int client, struct sockaddr_in* clientAddr)
{
	char request[256];
	struct message* msg = malloc(sizeof(struct message));
	size_t termlen = strlen(TERMINATION);
	while (1) {
		if (get_request(request, client) == -1) {
			perror("Error while receiving message.");
			break;
		}
		if (strlen(request) >= 5 && !strncmp(request, TERMINATION, (int) termlen)) {
			printf("Clients requests end of communication.\n");
			break;
		}
		if (message_from_string(msg, request) == -1) {
			perror("Error when parsing request.");
			break;
		}
		printf("%c%d:%s", msg->type, msg->code, msg->text);
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
	strcpy(request, buffer);
	return 0;
}
