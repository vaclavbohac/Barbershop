#include <stdio.h>

#include "help.h"

void help(const char* progname)
{
	printf("Usage: %s [--help] [--host=<host>] [--port=<port>]\n", progname);
	printf("\nOptions:\n  --help\tprints help\n  --host=<host>\tsets host, eg. localhost\n  --port=<port>\tsets port, eg. 8080\n");
}
