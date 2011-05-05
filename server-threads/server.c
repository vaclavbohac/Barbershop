#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/param.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "protocol/server.h"

void *handleClient(void *arg);

void server_start(struct server* srv)
{
	int client = 0;

	struct sockaddr_in clientAddr;
	size_t clientSize = sizeof(clientAddr);

	pthread_t id;

	while (1) {
		client = accept(srv->socket,
				(struct sockaddr*)&clientAddr, &clientSize);
		if (client == -1) {
			perror("Error while accepting connection.");
			continue;
		}
		printf("Server: Accepted client: '%d'.\n", client);
		pthread_create(&id, NULL, handleClient, (void *) client);
		pthread_detach(id);
	}
}

void *handleClient(void *arg)
{
	int client = (int) arg;
	char buffer[256];
	int length = read(client, buffer, sizeof(buffer));
	buffer[length] = '\0';
	printf("<= %s", buffer);
	printf("Server: Closing connection with client %d\n", client);
	close(client);
	pthread_exit((void *) 0);
}
