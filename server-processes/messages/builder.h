#ifndef MESSAGES_BUILDER_H
#define MESSAGES_BUILDER_H

#include "types.h"
#include "special.h"

int is_type(char* message, const char type);

char* message(const char type, int code, const char* message);

#endif
