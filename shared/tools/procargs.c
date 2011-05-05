#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools/help.h"
#include "tools/procargs.h"
#include "protocol/server.h"

void process_args(struct server* s, const char** args, const int length)
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
			s->port = atoi(buf);
		}
	}
}
