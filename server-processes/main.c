#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "help.h"

#include "messages/builder.h"

#define DEFAULT_PORT 4242

int global_port = DEFAULT_PORT;

void process_args(const char** args, const int length)
{
	char buf[128], *port = "--port=";
	int i, n = (int) strlen(port);
	for (i = 0; i < length; i++) {
		if (!strcmp(args[i], "--help")) {
			sprintf(buf, "%.*s", (int) strlen(args[0]) - 2, args[0] + 2);
			help(buf);
			exit(EXIT_SUCCESS);
		}
		if (!strncmp(args[i], port, n)) {
			sprintf(buf, "%.*s", 4, args[i] + n);
			global_port = atoi(buf);
		}
	}
}

int main(const int argc, const char* argv[])
{
	process_args(argv, argc);

	struct server* s = malloc(sizeof(struct server)); 
	if (server_init(s, global_port) == -1) {
		free(s);
		return EXIT_FAILURE;
	}

	server_start(s);

	server_stop(s);
	free(s);
	return EXIT_SUCCESS;
}
