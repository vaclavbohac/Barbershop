#ifndef PROTOCOL_CLIENT_H
#define PROTOCOL_CLIENT_H

struct client {
	char host[128];
	int handle;
	int time;
	int port;
};

#endif
