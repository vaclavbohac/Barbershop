#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "help.h"

void process_args(struct client* cli, const char* args[], int length)
{
	char *port = "--port=",
	     *host = "--host=",
	     *time = "--time=";
	int i,
	    p_length = strlen(port),
	    h_length = strlen(host),
	    t_length = strlen(time);

	for (i = 0; i < length; i++) {
		if (!strcmp(args[i], "--help")) {
			help(args[0] + 2); // Cuts './' from prog. name.
			exit(EXIT_SUCCESS);
		}
		if(!strncmp(args[i], host, h_length)) {
			strcpy(cli->host, args[i] + h_length);
			continue; // Skip next test.
		}
		if(!strncmp(args[i], port, p_length)) {
			cli->port = atoi(args[i] + p_length);
			continue;
		}
		if(!strncmp(args[i], time, t_length)) {
			cli->time = atoi(args[i] + t_length);
		}
	}
}
