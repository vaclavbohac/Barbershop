#include <stdlib.h>
#include <stdio.h>
#include "help.h"

void help(const char* progname)
{
	printf("Usage: %s [--help] [--port=<port>]\n", progname);
	printf("\nOptions:\n  --help\tprints help\n  --port=<port>\tsets port, eg. 8080\n");
}
