#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "response.h"

int send_response(struct client* cli, struct message* msg)
{
	char buffer[256];
	int ret, length = 0;

	// Convert message to string.
	length = sprintf(buffer, "%s\n", build_from_struct(msg));

	// Set null terminator.
	buffer[length] = '\0';

	// Send buffered data to client.
	ret = write(cli->handle, buffer, length);
	if (ret == -1) {
		// Error while sending response.
		fprintf(stderr, "Error while sending response.\n");
		return -1;
	}

#ifdef DEBUG
	printf("Sent %d bytes of data to %d client.\n",
			ret, cli->handle);
#endif

	return 0;
}

int get_response(struct client* cli, struct message* msg)
{
	char buffer[256];
	int  length,
	     bufsize = (int) sizeof(buffer);

	// Read data from server.
	length = read(cli->handle, buffer, bufsize);
	if (length == -1) {
		// Error while reading from server.
		fprintf(stderr, "Error while reading from server.\n");	
		return -1;
	}

	if (length == 0) {
		// Server closed connection message.
		printf("Server closed connection.\n");
		return -1;
	}

#ifdef DEBUG
	printf("Read %d bytes from server %d.\n",
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
