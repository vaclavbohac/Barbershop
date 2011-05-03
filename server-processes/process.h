#ifndef PROCESS_H
#define PROCESS_H

#include <arpa/inet.h>

#include "messages/builder.h"
#include "protocol/client.h"

void process(int client, struct sockaddr_in* clientAddr);

int handle_request(struct client* cli, struct message* request);

#endif
