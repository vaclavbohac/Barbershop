#include <stdio.h>
#include <string.h>

#include "builder.h"

int is_type(char* message, const char type)
{
	if (strlen(message) && message[0] == type) {
		return 1; 
	}
	return 0;
}


int is_type_from_struct(struct message* msg, const char type)
{
	return msg->type == type;
}


char* build(const char type, int code, const char* text)
{
	char buf[256];
	sprintf(buf, "%c[%02o]%c[%s]%c", type, code, SEPARATOR, text, ENDMESSAGE);
	return strdup(buf);
}


char* build_from_struct(struct message* msg)
{
	return build(msg->type, msg->code, msg->text);
}


void message_init(struct message* msg, const char type, const int code, const char* text)
{
	msg->type = type;
	msg->code = code;
	msg->text = strdup(text);
}
