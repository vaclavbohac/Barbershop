#ifndef RESPONSE_H
#define RESPONSE_H

#include "messages/builder.h"
#include "protocol/client.h"

int get_response(struct client* cli, struct message* msg);

int send_response(struct client* cli, struct message* msg);

#endif
