#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builder.h"

void message_init(struct message* msg, const char type, const int code, const char* text)
{
	msg->type = type;
	msg->code = code;
	msg->text = strdup(text);
}


char* build(const char type, int code, const char* text)
{
	char buf[256];
	sprintf(buf, "%c%02o%c%s", type, code, SEPARATOR, text);
	return strdup(buf);
}


char* build_from_struct(struct message* msg)
{
	return build(msg->type, msg->code, msg->text);
}


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


// Message Format: ?NN:text\n
//                 ^^ ^^   ^
//                 12 34   5
// 1. Message type.
// 2. Message code - optional.
// 3. Separator
// 4. Message body.
// 5. Ends message.
int message_from_string(struct message* msg, const char* string)
{
	int code = 0;
	char type, *text = "";
	if (!is_valid_type(string[0])) {
		return -1;
	}
	type = string[0];
	if (has_code(string)) {
		code = get_code(string);
	}
	if (has_text(string)) {
		text = get_text(string);
	}
	message_init(msg, type, code, text);
	return 0;
}


int has_text(const char* s)
{
	int i, len;
	for (i = 0, len = strlen(s); i < len; i++) {
		if (s[i] == SEPARATOR
			&& (i + 1) < (len - 2)) {
			return 1;
		}
	}
	return 0;
}


char* get_text(const char* s)
{
	char buf[256];
	int i, len;
	for (i = 0, len = strlen(s); i < len; i++) {
		if (s[i] == SEPARATOR) {
			int start = i + 1,
			    n = len - start;
			strncpy(buf, s + start, n);
			buf[n] = '\0';
			return strdup(buf);
		}
	}
	return "";
}


int has_code(const char* s)
{
	return (int) strlen(s) >= 3
		&& s[1] >= '0'
		&& s[1] <  '8'
		&& s[2] >= '0'
		&& s[2] <  '8';
}


int get_code(const char* s)
{
	char buf[16];
	strncpy(buf, s + 1, 2);
	buf[2] = '\0';
	return atoi(buf);
}


int is_valid_type(char type)
{
	return type == COMMAND	||
		type == ANSWER	||
		type == WARNING ||
		type == ERROR	||
		type == INFORMATION;
}
