#ifndef PROCESS_H
#define PROCESS_H

#include <arpa/inet.h>
#include "messages/builder.h"

void process(int client, struct sockaddr_in* clientAddr);


#endif
