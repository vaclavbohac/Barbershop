#ifndef PROTOCOL_SERVER_H
#define PROTOCOL_SERVER_H

#include <arpa/inet.h>

struct server {
	int socket;
	int port;
};

int server_init(struct server* srv, unsigned int port);

void server_start(struct server* srv);

void server_stop(struct server* srv);

void address_init(struct sockaddr_in* addr, unsigned int port);

#endif
