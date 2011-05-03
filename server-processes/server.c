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

#include "server.h"
#include "process.h"

#define MAX_CLIENTS 10

int server_init(struct server* srv, unsigned int port)
{
	int ret, opt, handle = socket(AF_INET, SOCK_STREAM, 0);
	if (handle == -1) {
		perror("Error when creating the server.");
		return -1;
	}
	opt = 1;
	ret = setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (ret == -1) {
		perror("Error when setting server reusability.");
		close(handle);
		return -1;
	}
	struct sockaddr_in addr;
	address_init(&addr, port);
	ret = bind(handle, (struct sockaddr *) &addr, sizeof(addr));  
	if (ret == -1) {
		perror("Error when binding the server.");
		close(handle);
		return -1;
	}
	listen(handle, MAX_CLIENTS);
	srv->socket = handle;
	srv->port = port;
	return 0;
}

void address_init(struct sockaddr_in* addr, unsigned int port)
{
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	addr->sin_addr.s_addr = INADDR_ANY;
}

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

		if (select(MAX(srv->socket, client) + 1, &read_wait_set, 0, 0, 0) == -1) {
			perror("Select error:");
			break;
		}
		
		if (FD_ISSET(srv->socket, &read_wait_set)) {
			client = accept(srv->socket, (struct sockaddr*)&clientAddr, &clientSize);
			if (client == -1) {
				perror("Error while accepting connection.");
			}
			printf("Accepted client: '%d'.\n", client);
#ifdef DEBUG
			printf("Creating new child.\n");
#endif
			pid = fork();
			if (pid == -1) {
				perror("Error while forking.");
			}
			else if (pid > 0) {
				// Close what is not needed.
				close(client);
				client = 0;
				waitpid(-1, NULL, WNOHANG);
			}
		}
		else if(FD_ISSET(client, &read_wait_set)) {
			process(client, &clientAddr);
			
			printf("Closing connection with client: '%d'.\n", client);
			close(client);
			client = 0;
		}
	}
}

void server_stop(struct server* srv)
{
	if ((void *)srv->socket != NULL) {
		close(srv->socket);
	}
}
