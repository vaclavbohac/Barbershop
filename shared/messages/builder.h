#ifndef MESSAGES_BUILDER_H
#define MESSAGES_BUILDER_H

#include "types.h"
#include "special.h"

struct message {
	int code;
	char type;
	char text[256];
};

int is_valid_type(char type);

int is_type(char* message, const char type);

int is_type_from_struct(struct message*, const char type);

char* build(const char type, int code, const char* text);

char* build_from_struct(struct message*);

int message_from_string(struct message*, const char* string);

void message_init(struct message*, const char type, const int code, const char* text); 

int has_text(const char* input_string);

char* get_text(const char* input_string);

int has_code(const char* input_string);

int get_code(const char* input_string);

#endif
