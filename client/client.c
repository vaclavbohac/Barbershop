#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "client.h"
#include "messages/builder.h"

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

	cli->socket = sock;
	cli->port = port;

	return 0;
}


int send_request(struct client* c, struct message* msg)
{
	char buffer[256];
	int len = sprintf(buffer, "%s", build_from_struct(msg)); 
	return write(c->socket, buffer, len);
}


int get_response(struct client* c, struct message* msg)
{
	char buffer[256];
	int length = read(c->socket, buffer, sizeof(buffer));
	buffer[length] = '\0';
	if (length == -1) {
		fprintf(stderr, "Error while reading from socket.\n");
		return -1;
	}
	return message_from_string(msg, buffer);
}


int client_start(struct client* cli)
{
#ifdef DEBUG
	printf("Client is running.\n");
#endif
	struct message* msg = (struct message*)
				malloc(sizeof(struct message));
	message_init(msg, COMMAND, 0, "enter");	// 1. Send enter request.
	if (send_request(cli, msg) == -1) {
		fprintf(stderr, "Error while sending request.\n");
		free(msg);
		return -1;
	}

	if (get_response(cli, msg) == -1) { // 2. Get response.
		fprintf(stderr, "Error while getting response.\n");
		free(msg);
		return -1;
	}

	printf("Message received: %s\n", msg->text);
	if (!strcmp("chairnotfree", msg->text)) {
		printf("Barbershop is full.\n");
	}
	else if(msg->type == INFORMATION && msg->code == 1) {
		// Chair is free.
		printf("%s\n", msg->text);
	}
	else {
		printf("Unknown message.\n");
	}

	free(msg);
	return 0;
}

void client_stop(struct client* cli)
{
	if ((void*)cli->socket != NULL) {
		close(cli->socket);	
	}
}
