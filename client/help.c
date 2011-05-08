#include <stdio.h>

#include "help.h"

void help(const char* progname)
{
	printf("Usage: %s [--help] [--host=<host>] [--port=<port>] [--time=<time>]\n", progname);
	printf("\nOptions:\n  --help\tprints help\n");
	printf("  --host=<host>\tsets host, eg. localhost\n");
	printf("  --port=<port>\tsets port, eg. 8080\n");
	printf("  --time=<time>\tsets time (note, it's octal value)\n");
}
