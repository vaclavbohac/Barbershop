#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "process.h"
#include "messages/builder.h"

void process(int client, struct sockaddr_in* clientAddr)
{
	char received[256];
	struct message* msg = malloc(sizeof(struct message));
	while (1) {
		if (get_message(received, client) == -1) {
			perror("Error while receiving message.");
			break;
		}
		if (strlen(received) >= 5 && !strncmp(received, TERMINATION, strlen(TERMINATION))) {
			printf("Clients requests end of communication.\n");
			break;
		}
		printf("2. Tested.\n");
		if (message_from_string(msg, received) == -1) {
			perror("Error when parsing request.");
			break;
		}
	}
}


int get_message(char* message, int client)
{
	char buffer[256];
	int length, bsize = sizeof(buffer);
	length = read(client, buffer, bsize);
	if (length == 0) {
		message = NULL;
		perror("Client closed connection.");
		return -1;
	}
	if (length == -1) {
		message = NULL;
		perror("Could not read from client.");
		return -1;
	}
	strcpy(message, buffer);
	printf("1. Copied\n");
	return 0;
}
