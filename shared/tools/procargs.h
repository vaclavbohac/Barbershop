#ifndef TOOLS_PROCESS_H
#define TOOLS_PROCESS_H

#include "protocol/server.h"

void process_args(struct server* s, const char** args, const int length);

#endif
