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

#include "protocol/process.h"
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
			return;
		}
		printf("Server: Accepted client: '%d'.\n", client);
		if (pthread_create(&id, NULL, handleClient,
				(void *) client)) {
			perror("Error while creating thread");
			return;
		}
		if (pthread_detach(id)) {
			perror("Error while detaching thread");
			return;
		}
	}
}

void *handleClient(void *arg)
{
	int client = (int) arg;
	process(client);
	printf("Server: Closing connection with client %d\n", client);
	close(client);
	pthread_exit((void *) 0);
}
