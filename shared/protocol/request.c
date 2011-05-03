#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "request.h"

int get_request(struct client* cli, struct message* msg)
{
	char buffer[256];
	int  length,
	     bufsize = (int) sizeof(buffer);

	// Read data from client.
	length = read(cli->handle, buffer, bufsize);
	if (length == -1) {
		// Error while reading from client.
		fprintf(stderr, "Error while reading from client.\n");	
		return -1;
	}

	if (length == 0) {
		// Client closed connection message.
		printf("Client closed connection.\n");
		return -1;
	}

#ifdef DEBUG
	printf("Read %d bytes from client %d.",
			length, cli->handle);
#endif

	// Set null terminator.
	buffer[length] = '\0';

	if (message_from_string(msg, buffer) == -1) {
		// Message is invalid.
		fprintf(stderr, "Error when parsing the message.\n");
		return -1;
	}

	return 0;
}


int send_request(struct client* cli, struct message* msg)
{
	char buffer[256];
	int ret, length = 0;

	// Convert message to string.
	length = sprintf(buffer, "%s\n", build_from_struct(msg));

	// Set null terminator.
	buffer[length] = '\0';

	// Send buffered data to server.
	ret = write(cli->handle, buffer, length);
	if (ret == -1) {
		// Error while sending request.
		fprintf(stderr, "Error while sending request.\n");
		return -1;
	}

#ifdef DEBUG
	printf("Sent %d bytes of data to %d server.\n",
			ret, cli->handle);
#endif

	return 0;
}
