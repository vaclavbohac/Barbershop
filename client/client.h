#ifndef CLIENT_H
#define CLIENT_H

#include "protocol/client.h"

int client_init(struct client* cli, char* host, int port);

int client_start(struct client* cli);

void client_stop(struct client* cli);

#endif
