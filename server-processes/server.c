#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "process.h"
#include "protocol/server.h"

void server_start(struct server* srv)
{
	int pid, client = 0;

	struct sockaddr_in clientAddr;
	size_t clientSize = sizeof(clientAddr);

	while (1) {
      		fd_set read_wait_set;

		// Empty read_wait_set.
		FD_ZERO(&read_wait_set);

		client == 0 ? 
			FD_SET(srv->socket, &read_wait_set) :
			FD_SET(client, &read_wait_set);

		if (select(MAX(srv->socket, client) + 1,
				&read_wait_set, 0, 0, 0) == -1) {
			perror("Select error");
			break;
		}
		
		if (FD_ISSET(srv->socket, &read_wait_set)) {
			client = accept(srv->socket,
				(struct sockaddr*)&clientAddr, &clientSize);
			if (client == -1) {
				perror("Error while accepting connection.");
			}
			printf("Server: Accepted client: '%d'.\n", client);
			pid = fork();
			if (pid == -1) {
				perror("Server: Error while forking.");
			}
			else if (pid > 0) {
				// Close what is not needed.
				close(client);
				client = 0;
			}
		}
		else if(FD_ISSET(client, &read_wait_set)) {
			// Process client request.
			process(client, &clientAddr);
			
			printf("Server: Closing connection with client: '%d'.\n", client);
			close(client);
			client = 0;

			exit(EXIT_SUCCESS);
		}
	}
}
