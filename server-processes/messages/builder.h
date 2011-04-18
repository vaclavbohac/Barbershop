#ifndef MESSAGES_BUILDER_H
#define MESSAGES_BUILDER_H

#include "types.h"
#include "special.h"

struct message {
	int code;
	char type;
	char* text;
};

int is_type(char* message, const char type);

int is_type_from_struct(struct message*, const char type);

char* build(const char type, int code, const char* text);

char* build_from_struct(struct message*);

void message_init(struct message*, const char type, const int code, const char* text); 

#endif
