#include "builder.h"
#include <stdio.h>
#include <string.h>

int is_type(char* message, const char type)
{
	if (strlen(message) && message[0] == type) {
		return 1; 
	}
	return 0;
}


char* message(const char type, int code, const char* message)
{
	char buf[256];
	int l = sprintf(buf, "%c[%02o]%c[%s]%c", type, code, SEPARATOR, message, ENDMESSAGE);
	if (l == -1) {
		perror("Error while creating message.");
		return "";
	}
	return strdup(buf);
}
