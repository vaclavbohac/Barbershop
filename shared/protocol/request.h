#ifndef REQUEST_H
#define REQUEST_H

#include "messages/builder.h"
#include "protocol/client.h"

int get_request(struct client* cli, struct message* msg);

int send_request(struct client* cli, struct message* msg);

#endif
